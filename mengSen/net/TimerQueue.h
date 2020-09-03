/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 21:12:58
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-03 21:57:14
 */

#ifndef __MENGSEN_NET_TIMERQUEUE_H__
#define __MENGSEN_NET_TIMERQUEUE_H__

#include <set>
#include <vector>

#include "../base/noncopyable.h"

namespace mengsen {

namespace net {

class EventLoop;
class Timer;
class TimerId;

class TimerQueue {
 public:
  TimerQueue(EventLoop*);
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TIMERQUEUE_H__