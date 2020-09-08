/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 21:21:44
 * @Last Modified by:   Mengsen.Wang
 * @Last Modified time: 2020-08-31 21:21:44
 */

#include "Channel.h"

#include <poll.h>

#include <sstream>

#include "../base/Logging.h"
#include "EventLoop.h"
#include "Poller.h"

namespace mengsen {

namespace net {

const int Channel::kNoneEvent = 0;
const int kReadEvent = POLLIN | POLLPRI;
const int kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      logHup_(true),
      tied_(false),
      eventHandling_(false),
      addedToLoop_(false) {}

Channel::~Channel() {
  assert(!eventHandling_);
  assert(!addedToLoop_);

  if (loop_->isInLoopThread()) {
    assert(!loop_->hasChannel(this));
  }
}

void Channel::handleEvent(uint64_t time) {
  std::shared_ptr<void> guard;
  if (tied_) {
    guard = tie_.lock();
    if (guard) {
      handleEventWithGuard(time);
    }
  } else {
    handleEventWithGuard(time);
  }
}

}  // namespace net

}  // namespace mengsen