/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 20:29:35
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-31 20:49:01
 */

#ifndef __MENGSEN_NET_TIMER_H__
#define __MENGSEN_NET_TIMER_H__

#include <atomic>

#include "../base/Timestamp.h"
#include "../base/noncopyable.h"
#include "Callbacks.h"

namespace mengsen {

namespace net {

class Timer : noncopyable {
 public:
  Timer(TimerCallback cb, uint16_t when, double interval)
      : callback_(std::move(cb)),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0),
        sequence_(++s_numCreated_) {}

  void run() const { callback_(); }

  uint64_t expiration() const { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }
  void restart(uint64_t now);

  static int64_t numCreated() { return s_numCreated_; }

 private:
  const TimerCallback callback_;
  uint64_t expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;
  static std::atomic<int64_t> s_numCreated_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMER_H__`