/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 16:59:42
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 22:45:47
 */

#include "Thread.h"

#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <memory>

#include "CurrentThread.h"
#include "Exception.h"

namespace mengsen {

namespace detail {

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void afterFork() {
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "main";
  CurrentThread::tid();
}

class ThreadNameInitializer {
 public:
  ThreadNameInitializer() {
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();

    pthread_atfork(nullptr, nullptr, &afterFork);
  }
};

static ThreadNameInitializer init;

struct ThreadData {
  using ThreadFunc = mengsen::Thread::ThreadFunc;
  ThreadFunc func_;
  std::string name_;
  pid_t* tid_;
  CountDownLatch* latch_;

  ThreadData(ThreadFunc func, const std::string& name, pid_t* tid,
             CountDownLatch* latch)
      : func_(std::move(func)), name_(name), tid_(tid), latch_(latch) {}

  void runInThread() {
    *tid_ = mengsen::CurrentThread::tid();
    tid_ = nullptr;
    latch_->countDown();
    latch_ = nullptr;

    CurrentThread::t_threadName =
        name_.empty() ? "mengsenThread" : name_.c_str();
    // set processed name
    ::prctl(PR_SET_NAME, mengsen::CurrentThread::t_threadName);
    try {
      func_();
      mengsen::CurrentThread::t_threadName = "finished";
    } catch (const Exception& ex) {
      mengsen::CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
      abort();
    } catch (const std::exception& ex) {
      CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
      fprintf(stderr, "reason: %s\n", ex.what());
      abort();
    } catch (...) {
      CurrentThread::t_threadName = "crashed";
      fprintf(stderr, "unknown exception caught int Thread %s\n",
              name_.c_str());
      throw;
    }
  }
};

void startThread(std::unique_ptr<detail::ThreadData>&& obj) {
  obj->runInThread();
}

}  // namespace detail

void CurrentThread::cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = detail::gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cachedTid);
  }
}

bool CurrentThread::isMainThread() {
  return CurrentThread::tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
  std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

std::atomic<int32_t> Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& name)
    : started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name_(name),
      latch_(1) {
  setDefaultName();
}

Thread::Thread(ThreadFunc func, std::string&& name)
    : started_(false),
      joined_(false),
      tid_(0),
      func_(std::move(func)),
      name_(name),
      latch_(1) {
  setDefaultName();
}

Thread::~Thread() {
  if (started_ && !joined_) threadObj.detach();
}

void Thread::setDefaultName() {
  int num = Thread::numCreated_++;
  if (name_.empty()) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Thread%d", num);
    name_ = buf;
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;
  std::unique_ptr<detail::ThreadData> data =
      std::make_unique<detail::ThreadData>(func_, name_, &tid_, &latch_);
  threadObj = std::thread(detail::startThread, std::move(data));
  if (threadObj.joinable()) {
    // until all ready and go on
    latch_.wait();
    assert(tid_ > 0);
  } else {
    started_ = false;
    // TODO add log
  }
}

void Thread::join() {
  assert(started_);
  assert(!joined_);
  if (threadObj.joinable()) {
    threadObj.join();
    joined_ = true;
  } else {
    // TODO add log
    threadObj.join();
  }
}

}  // namespace mengsen