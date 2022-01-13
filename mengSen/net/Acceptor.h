/*
 * @Date: 2022-01-13 08:23:59
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-01-13 10:44:13
 */

#ifndef __MENGSEN_NET_ACCEPTOR_H__
#define __MENGSEN_NET_ACCEPTOR_H__

#include <functional>

#include "Channel.h"
#include "Socket.h"

namespace mengsen {
namespace net {
class EventLoop;
class InetAddress;

class Acceptor : noncopyable {
 public:
  typedef std::function<void(int sockfd, const InetAddress&)>
      NewConnectionCallback;
  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();

  /**
   * @brief Set the New Connection Callback object
   * @param cb [NewConnectionCallback&]
   * @return void
   */
  void setNewConnectionCallback(const NewConnectionCallback& cb) {
    newConnectionCallback_ = cb;
  }

  void listen();

  bool listening() const { return listening_; }

 private:
  void handleRead();

  EventLoop* loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallback newConnectionCallback_;
  bool listening_;
  int idleFd_;
};
}  // namespace net
}  // namespace mengsen

#endif  // __MENGSEN_NET_ACCEPTOR_H__