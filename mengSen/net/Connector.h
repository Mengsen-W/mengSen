/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-29 21:32:32
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-29 21:44:45
 */

#ifndef __MENGSEN_NET_SOCKETOPS_H__
#define __MENGSEN_NET_SOCKETOPS_H__

#include <functional>
#include <memory>

#include "../base/noncopyable.h"
#include "InetAddress.h"

namespace mengsen {
namespace net {

class Channel;
class EventLoop;

class Connector : noncopyable, public std::enable_shared_from_this<Connector> {
 public:
  typedef std::function<void(int sockfd)> NewConnectionCallback;
  Connector(EventLoop* loop, const InetAddress& serverAddr);
  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb) {
    newConnectionCallback_ = cb;
  }

  void start();
  void restart();
  void stop();

 private:
  enum class States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30 * 1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { state_ = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  int removeAndResetChannel();
  void resetChannel();

  EventLoop* loop_;
  InetAddress serverAddr_;
  bool connect_;
  States state_;

  std::unique_ptr<Channel> channel_;
  NewConnectionCallback newConnectionCallback_;
  int retryDelayMs_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_CONNECTOR_H__