/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-01 19:57:34
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-01 19:59:49
 */

#ifndef __MENGSEN_NET_POLLER_H__
#define __MENGSEN_NET_POLLER_H__

#include <map>
#include <vector>

#include "../base/noncopyable.h"
#include "EventLoop.h"

namespace mengsen {

namespace net {

class Channel;

/**
 * @brief base class for IO multiplexing, doesn't own the Channel class
 */
class Poller : noncopyable {
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop*);
  virtual ~Poller();

  virtual uint64_t poll(int timeoutMs, ChannelList* activeChannels) = 0;
  virtual void updateChannel(Channel* channel) = 0;
  virtual void removeChannel(Channel* channel) = 0;
  virtual bool hasChannel(Channel* channel) = 0;

  static Poller* newDefaultPoller(EventLoop* loop);

  void assertInLoopThread() const { ownerloop_->assertInLoopThread(); }

 protected:
  typedef std::map<int, Channel*> ChannelMap;
  ChannelMap channels_;

 private:
  EventLoop* ownerloop_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_POLLER_H__