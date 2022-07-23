/*
 * @Date: 2020-09-29 21:50:27
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 10:57:01
 * @FilePath: /mengsen/mengsen/net/Acceptor.h
 */

#ifndef __MENGSEN_NET_ACCEPTOR_H__
#define __MENGSEN_NET_ACCEPTOR_H__

#include <functional>

#include "mengsen/net/Channel.h"
#include "mengsen/net/Socket.h"

namespace mengsen {
namespace net {
class EventLoop;
class InetAddress;

class Acceptor : NonCopyable {
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

  /**
   * @brief start acceptor listening
   */
  void listen();

  /**
   * @brief get listening state
   * @return bool
   */
  bool listening() const { return listening_; }

 private:
  /**
   * @brief new connection callback
   */
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
