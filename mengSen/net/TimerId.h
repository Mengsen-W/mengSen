/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 20:59:02
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-31 21:12:24
 */

#ifndef __MENGSEN_NET_TIMERID_H__
#define __MENGSEN_NET_TIMERID_H__

#include <stdint.h>

#include "../base/copyable.h"

namespace mengsen {

namespace net {

class Timer;

class TimerId : copyable {
 public:
  TimerId() : timer_(nullptr), sequence_(0) {}
  TimerId(Timer* time, int64_t seq) : timer_(time), sequence_(seq) {}
  friend class TimeQueue;

 private:
  Timer* timer_;
  int64_t sequence_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMERID_H__