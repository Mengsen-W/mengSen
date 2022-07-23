/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-29 21:45:13
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-29 21:46:24
 */

#ifndef __MENGSEN_NET_TCPCLIENT_H__
#define __MENGSEN_NET_TCPCLIENT_H__

#include "mengsen/base/Mutex.h"
#include "mengsen/net/TcpConnection.h"
namespace mengsen {
namespace net {

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : NonCopyable {
 public:
  /**
   * @param loop [EventLoop*]
   * @param serverAddr [InetAddress&]
   * @param nameArg [const std::string&] client name
   */
  TcpClient(EventLoop* loop, const InetAddress& serverAddr,
            const std::string& nameArg);

  /**
   *  force out-line dtor, for std::unique_ptr members.
   */
  ~TcpClient();

  /**
   * connect to server
   */
  void connect();

  /**
   * disconnect to server, shutdown local fd
   */
  void disconnect();

  /**
   * stop client, close local fd
   */
  void stop();

  /**
   * get TcpConnection
   * @return [TcpConnectionPtr]
   */
  TcpConnectionPtr connection() const {
    MutexLockGuard lock(mutex_);
    return connection_;
  }

  /**
   * get bind loop
   * @return [EventLoop*]
   */
  EventLoop* getLoop() const { return loop_; }

  /**
   * get retry flag
   * @return bool
   */
  bool retry() const { return retry_; }

  /**
   * set retry flag to true
   */
  void enableRetry() { retry_ = true; }

  /**
   * get TcpClient name
   * @return const std::string&
   */
  const std::string& name() const { return name_; }

  /**
   * Set connection callback. Not thread safe.
   * @param cb [const ConnectionCallback&]
   */
  void setConnectionCallback(const ConnectionCallback& cb) {
    connectionCallback_ = cb;
  }

  /**
   * Set message callback. Not thread safe.
   * @param cb [const MessageCallback&]
   */
  void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

  /**
   * Set write complete callback. Not thread safe.
   * @param cb [const WriteCompleteCallback&]
   */
  void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
    writeCompleteCallback_ = cb;
  }

 private:
  /**
   * callback by connector when connection writeable every time. Not thread
   * safe, but in loop
   */
  void newConnection(int sockfd);
  /**
   * callback by Not
   * thread safe, but in loop
   */
  void removeConnection(const TcpConnectionPtr& conn);

  EventLoop* loop_;
  ConnectorPtr connector_;  // avoid revealing Connector
  const std::string name_;
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  // FIXME: use atomic variable if multi thread
  bool retry_;
  bool connect_;
  // always in loop thread
  int nextConnId_;
  mutable MutexLock mutex_;
  TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_TCPCLIENT_H__