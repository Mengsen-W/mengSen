/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 21:21:46
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-01 17:53:08
 */

#ifndef __MENGSEN_NET_CHANNEL_H__
#define __MENGSEN_NET_CHANNEL_H__

#include <functional>
#include <memory>

#include "../base/noncopyable.h"

namespace mengsen {

namespace net {

class EventLoop;

/**
 * @brief a selectable I/O channel
 * This class doesn't own the file descriptor.
 * The file descriptor could be a socket, an eventfd, a timerfd, or a signalfd
 */
class Channel : noncopyable {
 public:
  typedef std::function<void()> EventCallback;
  typedef std::function<void(uint64_t time)> ReadEventCallback;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  void handleEvent(uint64_t receiveTime);

  void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

  /**
   * @brief Tie this channel to the owner object managed by shared_ptr, prevent
   * the owner object being destroyed in handleEvent.
   * @param handleEvent [std::shared_ptr<void>&]
   */
  void tie(const std::shared_ptr<void>&);
  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(int revt) { revents_ = revt; }  // used by pollers
  // int revents() const { return revents_; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }
  void enableReading() {
    events_ |= kReadEvent;
    update();
  }
  void disableReading() {
    events_ &= ~kReadEvent;
    update();
  }
  void enableWriting() {
    events_ |= kWriteEvent;
    update();
  }
  void disableWriting() {
    events_ &= ~kWriteEvent;
    update();
  }
  void disableAll() {
    events_ = kNoneEvent;
    update();
  }
  bool isWriting() const { return events_ & kWriteEvent; }
  bool isReading() const { return events_ & kReadEvent; }

  // for Poller
  int index() { return index_; }
  void set_index(int idx) { index_ = idx; }
  // for debug
  std::string reventsToString() const;
  std::string eventsToString() const;
  void doNotLogHup() { logHup_ = false; }
  EventLoop* ownerLoop() { return loop_; }
  void remove();

 private:
  static std::string eventToString(int fd, int ev);
  void update();
  void handleEventWithGuard();

 private:
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
  EventLoop* loop_;
  const int fd_;
  int events_;
  int revents_;  // it's the received event types of epoll or poll
  int index_;    // used by Poller.
  bool logHup_;
  std::weak_ptr<void> tie_;
  bool tied_;
  bool eventHandling_;
  bool addedToLoop_;
  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_CHANNEL_H__