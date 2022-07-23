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

#include "mengsen/base/NonCopyable.h"
#include "mengsen/net/Callbacks.h"
#include "mengsen/net/Channel.h"

namespace mengsen {

namespace net {

class EventLoop;
class Timer;
class TimerId;

/**
 * @brief Timer queue with a EventLoop*
 */
class TimerQueue {
 public:
  /**
   * @param ev [EventLoop*] this TimerQueue EventLoop
   */
  TimerQueue(EventLoop*);
  ~TimerQueue();

  /**
   * @brief add Timer to this EventLoop
   * @param cb [TimerCallback] construct TimerId
   * @param time [Timestamp::Time_uint64_t]
   * @param interval [double]
   * @return [TimerId] create TimerId
   */
  TimerId addTimer(TimerCallback cb, Timestamp::Time_uint64_t time, double interval);

  /**
   * @brief cancel TimerId
   * @param timeid [TimerId]
   */
  void cancel(TimerId timeid);

 private:
  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  // This requires heterogeneous comparison lookup (N3465) from C++14
  // so that we can find an T* in a set<unique_ptr<T>>.

  // time and Timer* pair
  typedef std::pair<Timestamp::Time_uint64_t, Timer*> Entry;
  // Entry set
  typedef std::set<Entry> TimerList;
  typedef std::pair<Timer*, Timestamp::Time_uint64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;

 private:
  /**
   * @brief add timer to timer list and it call insert()
   * @param timer [Timer*]
   */
  void addTimerInLoop(Timer* timer);

  /**
   * @brief user TimerId cancel timer
   * @param timerId [TimerId]
   */
  void cancelInLoop(TimerId timerId);

  /**
   * @brief called when timerfd alarms, this function will call timer callback
   */
  void handleRead();

  /**
   * @brief move out all expired timers
   * @param now [Timestamp::Time_uint64_t]
   * @return [std::vector<Entry>]
   */
  std::vector<Entry> getExpired(Timestamp::Time_uint64_t now);

  /**
   * @brief reset timer to timer list, cancel or repeat
   * @param expired [std::vector<Entry>]
   * @param now [Timestamp::Time_uint64_t]
   */
  void reset(const std::vector<Entry>& expired, Timestamp::Time_uint64_t now);

  /**
   * @brief insert Timer to this timers_ and activeTimers_
   * @param timer [Timer*]
   * @return [bool] true if set is empty or timer's expiration is earlier than
   * the first timer
   */
  bool insert(Timer* timer);

 private:
  // TimerQueue for EventLoop
  EventLoop* loop_;
  // timer fd
  const int timerfd_;
  // timer channel
  Channel timerfdChannel_;
  // Timer list sorted by expiration
  TimerList timers_;
  // for cancel()
  ActiveTimerSet activeTimers_;
  // atomic
  bool callingExpiredTimers_;
  // self-cancel active Timers set
  ActiveTimerSet cancelingTimers_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMERQUEUE_H__