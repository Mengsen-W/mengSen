/*
 * @Date: 2020-08-31 20:29:35
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 12:58:54
 * @FilePath: /mengsen/mengsen/net/Timer.h
 */

#ifndef __MENGSEN_NET_TIMER_H__
#define __MENGSEN_NET_TIMER_H__

#include <atomic>

#include "mengsen/base/NonCopyable.h"
#include "mengsen/base/Timestamp.h"
#include "mengsen/net/Callbacks.h"

namespace mengsen {

namespace net {

/**
 * @brief internal class for timer event
 */
class Timer : NonCopyable {
 public:
  /**
   * @param cb [TimerCallback]
   * @param when [Timestamp::Time_uint64_t] timestamp for next wake
   * @param interval [double] is s seconds for repeat time
   */
  Timer(TimerCallback cb, Timestamp::Time_uint64_t when, double interval)
      : callback_(std::move(cb)),
        expiration_(when),
        interval_(interval),
        repeat_(interval > 0.0),
        sequence_(++s_numCreated_) {}

  /**
   * @brief run callback
   */
  void run() const { callback_(); }

  /**
   * @brief get expiration time
   */
  Timestamp::Time_uint64_t expiration() const { return expiration_; }

  /**
   * @brief get repeat bool
   */
  bool repeat() const { return repeat_; }

  /**
   * @brief get sequence
   */
  int64_t sequence() const { return sequence_; }

  /**
   * @brief restart from now
   * @param now [Timestamp::Time_uint64_t]
   */
  void restart(Timestamp::Time_uint64_t now);

  /**
   * @brief get created number
   * @return [int64_t]
   */
  static int64_t numCreated() { return s_numCreated_; }

 private:
  // callback function
  const TimerCallback callback_;

  // expiration time
  Timestamp::Time_uint64_t expiration_;

  // interval time
  const double interval_;

  // repeat bool
  const bool repeat_;

  // sequence id
  const int64_t sequence_;

  // created timer number
  static std::atomic<int64_t> s_numCreated_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMER_H__`