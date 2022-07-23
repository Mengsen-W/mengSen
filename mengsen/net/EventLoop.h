/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-01 17:36:18
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-03 21:56:08
 */

#ifndef __MENGSEN_NET_EVENTLOOP_H__
#define __MENGSEN_NET_EVENTLOOP_H__

#include <atomic>
#include <any>
#include <functional>
#include <memory>
#include <mutex>

#include "mengsen/base/CurrentThread.h"
#include "mengsen/base/NonCopyable.h"
#include "mengsen/net/Callbacks.h"
#include "mengsen/net/TimerId.h"

namespace mengsen {

namespace net {

class Channel;
class Poller;
class TimerQueue;

/**
 * @brief every I/O thread has one EventLoop
 */
class EventLoop : NonCopyable {
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

  Timestamp::Time_uint64_t pollReturnTime() const { return pollReturnTime_; }

  int64_t iteration() const { return iteration_; }

  /**
   * @brief run callback function in the loop thread, if call this in loop
   * thread, that execute the cb at now, else other thread used loop pointer or
   * loop object execute the function which call queueInLoop put callback
   * function in the pending queue.
   * @param cb callback function
   */
  void runInLoop(Functor cb);

  /**
   * @brief push callback function in pending queue, and wakeup loop thread
   * execute callback function
   * @param cb callback function
   */
  void queueInLoop(Functor cb);

  /**
   * @brief get queue size of pending queue
   * @return queue size
   */
  size_t queueSize() const;

  /**
   * @brief run callback function at a certain time
   * @param time timestamp
   * @param cb callback function
   * @return timer id
   */
  TimerId runAt(Timestamp::Time_uint64_t time, TimerCallback cb);

  /**
   * @brief run callback function at now time + delay
   * @param delay time delay
   * @param cb callback function
   * @return timer id
   */
  TimerId runAfter(double delay, TimerCallback cb);

  /**
   * @brief run callback function every interval
   * @param interval time interval
   * @param cb callback function
   * @return timer id
   */
  TimerId runEvery(double interval, TimerCallback cb);

  void cancel(TimerId timerId);
  // internal usage

  /**
   * @brief wakeup loop thread, read a byte to wakeupFd_
   */
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

  /**
   * @brief is in IO loop thread from create thread function
   * @return bool
   */
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

  // bool callingPendingFunctors() const { return callingPendingFunctors_; }
  bool eventHandling() const { return eventHandling_; }
  void setContext(const std::any& context) { context_ = context; }
  const std::any& getContext() const { return context_; }
  std::any* getMutableContext() { return &context_; }
  static EventLoop* getEventLoopOfCurrentThread();

 private:
  void abortNotInLoopThread();

  /**
   * @brief wakeup loop thread, use this callback read a byte to wakeupFd_
   */
  void handleRead();
  void doPendingFunctors();

  void printActiveChannels() const;

  typedef std::vector<Channel*> ChannelList;

  // is lopping flag
  bool looping_;

  // quit flag maybe change in different thread
  std::atomic<bool> quit_;

  // eventHandling flag
  bool eventHandling_;
  bool callingPendingFunctors_;

  int64_t iteration_;
  const pid_t threadId_;
  Timestamp::Time_uint64_t pollReturnTime_;
  std::unique_ptr<Poller> poller_;
  std::unique_ptr<TimerQueue> timerQueue_;

  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;

  std::any context_;

  ChannelList activeChannels_;
  Channel* currentActiveChannel_;

  mutable std::mutex mutex_;
  std::vector<Functor> pendingFunctors_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_EVENTLOOP_H__