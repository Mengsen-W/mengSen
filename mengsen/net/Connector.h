/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-29 21:32:32
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-29 21:44:45
 */

#ifndef __MENGSEN_NET_CONNECTOR_H__
#define __MENGSEN_NET_CONNECTOR_H__

#include <atomic>
#include <functional>
#include <memory>

#include "mengsen/base/NonCopyable.h"
#include "mengsen/net/InetAddress.h"

namespace mengsen {
namespace net {

class Channel;
class EventLoop;

/**
 * TcpConnect user establish a connection to a remote host. Conntor used in
 * client Initiate connection
 */
class Connector : NonCopyable, public std::enable_shared_from_this<Connector> {
 public:
  typedef std::function<void(int sockfd)> NewConnectionCallback;

  /**
   * constructor Connector
   * @param: loop [EventLoop*] bind eventloop
   * @param: serverAddr [const InetAddress&] peer server address
   */
  Connector(EventLoop* loop, const InetAddress& serverAddr);
  ~Connector();

  /**
   * set new connection callback, if connect to peer sever
   * @param cb [const NewConnectionCallback&] socket fd will pass to cb
   */
  void setNewConnectionCallback(const NewConnectionCallback& cb) {
    newConnectionCallback_ = cb;
  }

  /**
   * start establish connection, an be called in any thread
   */
  void start();

  /**
   * restart establish connection, must be called in loop thread
   */
  void restart();  // must be called in loop thread

  /**
   * stopestablish connection, an be called in any thread
   */
  void stop();

  /**
   * get server address
   * @return const InetAddress&
   */
  const InetAddress& serverAddress() const { return serverAddr_; }

 private:
  // connect state
  enum States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30 * 1000;  // max retry connect time
  static const int kInitRetryDelayMs = 500;       // init retry connect time

  /**
   * set connector state
   * @param state [States]
   */
  void setState(States s) { state_ = s; }

  /**
   * start inner function for loop
   */
  void startInLoop();

  /**
   * stop inner function for loop
   */
  void stopInLoop();

  /**
   * connect actually inner function, called by startInLoop
   */
  void connect();

  /**
   * change state to kConnecting and bind channel callback
   * @param sockfd [int] local socket fd
   */
  void connecting(int sockfd);

  /**
   * bind channel write callback
   */
  void handleWrite();

  /**
   * bind channel error callback
   */
  void handleError();

  /**
   * close fd and if connect_ is true than retry
   * @param sockfd [int] local sockfd
   */
  void retry(int sockfd);

  /**
   * remove and reset channel_
   * @return int channel_'s fd
   */
  int removeAndResetChannel();

  /**
   * reset channel_
   */
  void resetChannel();

  EventLoop* loop_;
  InetAddress serverAddr_;
  // FIXME: if multiple thread, use atomic
  bool connect_;
  States state_;
  std::unique_ptr<Channel> channel_;
  NewConnectionCallback newConnectionCallback_;
  int retryDelayMs_;  // actually retry time
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_CONNECTOR_H__