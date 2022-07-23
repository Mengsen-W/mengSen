/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-09 20:25:24
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-09 20:30:56
 */

#include "Poller.h"

#include "Channel.h"

namespace mengsen {

namespace net {

Poller::Poller(EventLoop* loop) : ownerloop_(loop) {}

Poller::~Poller() = default;

bool Poller::hasChannel(Channel* channel) const {
  assertInLoopThread();
  ChannelMap::const_iterator it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

}  // namespace net

}  // namespace mengsen