/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-13 21:24:37
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 20:50:44
 */

#include "Thread.h"

#include <error.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>

#include "CurrentThread.h"
#include "Exception.h"
#include "Timestamp.h"

namespace mengsen {

namespace detail {

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void afterFork() {
  // init cacheTid
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "main";
  // cached t_cachedTid
  CurrentThread::tid();
}

class ThreadNameInitializer {
 public:
  ThreadNameInitializer() {
    CurrentThread::t_threadName = "main";
    // cached t_cachedTid
    CurrentThread::tid();
    /**
     * int pthread_atfork(void (*prepare)(void), void (*parent)(void), void
     * (*child)(void));
     * before fork parent will call prepare and after fork parent will call
     * parent and child call child
     */
    pthread_atfork(NULL, NULL, &afterFork);
  }
};
// FIXME :scope of all
ThreadNameInitializer init;

/**
 * @brief Thread Date struct
 */
struct ThreadDate {
  typedef Thread::ThreadFunc ThreadFunc;
  ThreadFunc func_;
  std::string name_;
  pid_t* tid_;
  CountDownLatch* latch_;

  ThreadDate(ThreadFunc func, const std::string& name, pid_t* tid,
             CountDownLatch* latch)
      : func_(std::move(func)), name_(name), tid_(tid), latch_(latch) {}

  void runInThread() {
    // recheck cached t_cachedTid
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

void* startThread(void* obj) {
  ThreadDate* data = static_cast<ThreadDate*>(obj);
  data->runInThread();
  delete data;
  return nullptr;
}

}  // namespace detail

void CurrentThread::cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = detail::gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
  }
}

bool CurrentThread::isMainThread() {
  return CurrentThread::tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec) {
  struct timespec ts = {0, 0};
  ts.tv_sec = static_cast<int64_t>(usec / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec =
      static_cast<int64_t>(usec % Timestamp::kMicroSecondsPerSecond * 1000);
  ::nanosleep(&ts, NULL);
}

std::atomic<int32_t> Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& n)
    : started_(false),
      joined_(false),
      pthreadId_(0),
      tid_(0),
      func_(std::move(func)),
      name_(n),
      latch_(1) {
  setDefaultName();
}

Thread::~Thread() {
  if (started_ && !joined_) {
    pthread_detach(pthreadId_);
  }
  // else directly destructor
}

void Thread::setDefaultName() {
  int num = numCreated_++;
  // set name
  if (name_.empty()) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Thread%d", num);
    name_ = buf;
  }
}

void Thread::start() {
  assert(!started_);
  started_ = true;
  detail::ThreadDate* data =
      new detail::ThreadDate(func_, name_, &tid_, &latch_);
  if (pthread_create(&pthreadId_, NULL, &detail::startThread, data)) {
    // on success return 0
    started_ = false;
    delete data;
    // TODO: add log
    // log
  } else {
    latch_.wait();
    assert(tid_ > 0);
  }
}

int Thread::join() {
  assert(started_);
  assert(!joined_);
  joined_ = true;
  return pthread_join(pthreadId_, NULL);
}

}  // namespace mengsen