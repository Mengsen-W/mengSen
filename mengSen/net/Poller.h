/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-01 19:57:34
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-01 19:59:49
 */

#ifndef __MENGSEN_NET_POLLER_H__
#define __MENGSEN_NET_POLLER_H__

#include "../base/noncopyable.h"

namespace mengsen {

namespace net {

class EventLoop;

class Poller : noncopyable {};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_POLLER_H__