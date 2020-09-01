/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-01 17:36:18
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-01 19:43:09
 */

#ifndef __MENGSEN_NET_EVENTLOOP_H__
#define __MENGSEN_NET_EVENTLOOP_H__

#include <functional>

#include "../base/noncopyable.h"

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

 private:
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_EVENTLOOP_H__