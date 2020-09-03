/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-01 17:36:18
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-03 21:56:08
 */

#ifndef __MENGSEN_NET_EVENTLOOP_H__
#define __MENGSEN_NET_EVENTLOOP_H__

#include <atomic>
#include <boost/any.hpp>
#include <functional>
#include <memory>
#include <mutex>

#include "../base/CurrentThread.h"
#include "../base/noncopyable.h"
#include "Callbacks.h"
#include "TimerId.h"

namespace mengsen {

namespace net {

class Channel;
class Poller;
class TimerQueue;

/**
 * @brief every I/O thread has one EventLoop
 */
class EventLoop : noncopyable {
 public:
  typedef std::function<void()> Functor;

  EventLoop();
  ~EventLoop();

  /**
   * @brief event loop with thread and must be called in the same thread as
   * creation of objection
   */
  void loop();

  /**
   * @brief quit loop. This is not 100% thread safe, if you call through a raw
   * pointer, better to call through shared_ptr<EventLoop> for 100% safety.
   */
  void quit();

  uint64_t pollReturnTime() const { return pollReturnTime_; }

  int64_t iteration() const { return iteration_; }

  void runInLoop(Functor cb);

  void queueInLoop(Functor cb);

  size_t queueSize() const;

  TimerId runAt(uint64_t time, TimerCallback cb);

  TimerId runAfter(double delay, TimerCallback cb);

  TimerId runEvery(double interval, TimerCallback cb);

  void cancel(TimerId timerId);
  // internal usage
  void wakeup();
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  bool hasChannel(Channel* channel);
  // pid_t threadId() const { return threadId_; }
  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  // bool callingPendingFunctors() const { return callingPendingFunctors_; }
  bool eventHandling() const { return eventHandling_; }
  void setContext(const boost::any& context) { context_ = context; }
  const boost::any& getContext() const { return context_; }
  boost::any* getMutableContext() { return &context_; }
  static EventLoop* getEventLoopOfCurrentThread();

 private:
  void abortNotInLoopThread();
  void handleRead();
  void doPendingFunctors();

  void printActiveChannels() const;

  typedef std::vector<Channel*> ChannelList;

  bool looping_;
  std::atomic<bool> quit_;
  bool eventHandling_;
  bool callingPendingFunctors_;
  int64_t iteration_;
  const pid_t threadId_;
  uint64_t pollReturnTime_;
  std::unique_ptr<Poller> poller_;
  std::unique_ptr<TimerQueue> timerQueue_;
  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  boost::any context_;

  ChannelList activeChannels_;
  Channel* currentActiveChannel_;

  std::mutex mutex_;
  std::vector<Functor> pendingFunctors_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_EVENTLOOP_H__