/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-03 21:28:53
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-03 22:01:46
 */

#include "EventLoop.h"

#include <signal.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "../base/Logging.h"
#include "Channel.h"
#include "Poller.h"

namespace mengsen {

namespace net {

namespace {
thread_local EventLoop* t_loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

int createEventfd() {
  int evf = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evf < 0) {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }

  return evf;
}

#pragma GCC diagnostic ignored "-Wold-style-cast"
class IgnoreSigPipe {
 public:
  IgnoreSigPipe() {
    ::signal(SIGPIPE, SIG_IGN);
    // LOG_TRACE << "Ignore SIGPIPE";
  }
};
#pragma GCC diagnostic error "-Wold-style-cast"
IgnoreSigPipe initObj;

}  // namespace

EventLoop* EventLoop::getEventLoopOfCurrentThread() {
  return t_loopInThisThread;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      eventHandling_(false),
      callingPendingFunctors_(false),
      iteration_(0),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      timerQueue_(std::make_unique<TimerQueue>(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(std::make_unique<Channel>(this, wakeupFd_)),
      currentActiveChannel_(nullptr) {
  LOG_DEBUG << "EventLoop create " << this << " in thread " << threadId_;
  if (t_loopInThisThread) {
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread
              << " exists in this thread " << threadId_;

  } else {
    t_loopInThisThread = this;
  }
  wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
  LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
            << " distructs in thread " << CurrentThread::tid();
  wakeupChannel_->disableAll();
  wakeupChannel_->remove();
  ::close(wakeupFd_);
  t_loopInThisThread = nullptr;
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  // FIXME what if someone calls quit() before loop()?
  quit_ = false;
  LOG_TRACE << "EventLoop " << this << " start looping";

  while (!quit_) {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    ++iteration_;
    if (Logger::logLevel() <= Logger::TRACE) {
      printActiveChannels();
    }

    // TODO sort channel by priority

    eventHandling_ = true;
    for (Channel* channel : activeChannels_) {
      currentActiveChannel_ = channel;
      currentActiveChannel_->handleEvent(pollReturnTime_);
    }
    currentActiveChannel_ = nullptr;
    eventHandling_ = false;
    doPendingFunctors();
  }

  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
  // There is a chance that loop() just executes while(!quit_) and exits,
  // then EventLoop destructs, then we are accessing an invalid object.
  // Can be fixed using mutex_ in both places.
  if (!isInLoopThread()) {
    wakeup();
  }
}

void EventLoop::runInLoop(Functor cb) {
  if (isInLoopThread()) {
    cb();
  } else {
    queueInLoop(std::move(cb));
  }
}

void EventLoop::queueInLoop(Functor cb) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
  }
  if (!isInLoopThread() || callingPendingFunctors_) {
    wakeup();
  }
}

size_t EventLoop::queueSize() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return pendingFunctors_.size();
}

}  // namespace net

}  // namespace mengsen