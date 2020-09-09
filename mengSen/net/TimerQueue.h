/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 21:12:58
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-09 21:36:04
 */

#ifndef __MENGSEN_NET_TIMERQUEUE_H__
#define __MENGSEN_NET_TIMERQUEUE_H__

#include <set>
#include <vector>

#include "../base/noncopyable.h"
#include "Callbacks.h"
#include "Channel.h"

namespace mengsen {

namespace net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue {
 public:
  TimerQueue(EventLoop*);
  ~TimerQueue();

  TimerId addTimer(TimerCallback cb, uint64_t time, double interval);
  void cancel(TimerId timeid);

 private:
  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  // This requires heterogeneous comparison lookup (N3465) from C++14
  // so that we can find an T* in a set<unique_ptr<T>>.
  typedef std::pair<uint64_t, Timer*> Entry;
  typedef std::set<Entry> TimerList;
  typedef std::pair<Timer*, int64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;
  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  // called when timerfd alarms
  void handleRead();
  // move out all expired timers
  std::vector<Entry> getExpired(uint64_t now);
  void reset(const std::vector<Entry>& expired, uint64_t now);
  bool insert(Timer* timer);
  EventLoop* loop_;
  const int timerfd_;
  Channel timerfdChannel_;
  // Timer list sorted by expiration
  TimerList timers_;
  // for cancel()
  ActiveTimerSet activeTimers_;
  bool callingExpiredTimers_; /* atomic */
  ActiveTimerSet cancelingTimers_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMERQUEUE_H__