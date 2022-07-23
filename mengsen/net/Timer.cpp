/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 20:29:58
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-31 20:50:43
 */

#include "mengsen/net/Timer.h"

namespace mengsen {

namespace net {

std::atomic<int64_t> Timer::s_numCreated_ = 0;

void Timer::restart(Timestamp::Time_uint64_t now) {
  if (repeat_) {
    expiration_ = Timestamp::addTime(now, interval_);

  } else {
    expiration_ = Timestamp::invalid();
  }
}

}  // namespace net

}  // namespace mengsen