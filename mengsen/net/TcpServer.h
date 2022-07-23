/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-29 21:47:24
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-29 21:47:51
 */

#ifndef __MENGSEN_NET_TCPSERVER_H__
#define __MENGSEN_NET_TCPSERVER_H__

#include <map>

#include "mengsen/base/Atomic.h"
#include "mengsen/base/Types.h"
#include "mengsen/net/TcpConnection.h"

namespace mengsen {
namespace net {

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer {
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;
  enum Option {
    kNoReusePort,
    kReusePort,
  };

  /**
   * @brief: constructor TcpServer
   * @param: loop [EventLoop*] listener socket loop
   * @param: listenAddr [const InetAddress&] listen address
   * @param: name [const string&] server name
   * @param: option [Option] kNoReusePort or kReusePort
   */
  TcpServer(EventLoop* loop, const InetAddress& listenAddr,
            const std::string& nameArg, Option option = kNoReusePort);

  /**
   * @brief: constructor TcpServer, reset TcpConnections shared_ptr and
   * than insert connectDestroyed call back to connection loop. force out-line
   * dtor, for std::unique_ptr members.
   */
  ~TcpServer();

  /**
   * @brief: get listen socket ip port combination
   * @return: [const std::string&]
   */
  const std::string& ipPort() const { return ipPort_; }

  /**
   * @brief: get listen socket name
   * @return: [const std::string&]
   */
  const std::string& name() const { return name_; }

  /**
   * @brief: get listen socket loop
   * @return: [EventLoop*]
   */
  EventLoop* getLoop() const { return loop_; }

  /**
   * @brief: set thread number of threads for handling input; always accepts new
   * connection in loop's thread. Must be called before TcpServer::start()
   * @param numThreads I/O loop's thread number
   * @param numThreads = 0 means all I/O in loop's thread, no thread will
   * created, this is the default value.
   * @param numThreads = 1 means all I/O in another thread.
   * @param numThreads = N means a thread poolwith N threads, new connections
   * are assigned on a round-robin basis.
   */
  void setThreadNum(int numThreads = 0);

  /**
   * @brief: set init callback function of thread from thread pool
   * @param cb [ThreadInitCallback&]
   */
  void setThreadInitCallback(const ThreadInitCallback& cb) {
    threadInitCallback_ = cb;
  }

  /**
   * @brief: get thread pool, after calling start() valid
   * @return [std::shared_ptr<EventLoopThreadPool>]
   */
  std::shared_ptr<EventLoopThreadPool> threadPool() { return threadPool_; }

  /**
   * @brief: start server if it's not listening. It's harmless to call it
   * multiple times, because use atomic variable, thread safe.
   */
  void start();

  /**
   * @brief: set connection callback, Not thread safe.
   * @param cb [const ConnectionCallback&]
   */
  void setConnectionCallback(const ConnectionCallback& cb) {
    connectionCallback_ = cb;
  }

  /**
   * @brief: set Message callback, Not thread safe.
   * @param cb [const MessageCallback&]
   */
  void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }

  /**
   * @brief: set write complete callback, Not thread safe.
   * @param cb [const WriteCompleteCallback&]
   */
  void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
    writeCompleteCallback_ = cb;
  }

 private:
  /**
   * @brief: acceptor new connection callback, not thread safe but in loop
   * @param sockfd [int] connection fd
   * @param peerAddr [const InetAddress&] peer address
   */
  void newConnection(int sockfd, const InetAddress& peerAddr);

  /**
   * @brief: remove connection from map, thread safe.
   * @param conn [const TcpConnectionPtr&]
   */
  void removeConnection(const TcpConnectionPtr& conn);

  /**
   * @brief: remove connection from map, push to TcpSever::loop_::runInLoop()
   * in TcpSever::removeConnection, Not thread safe, but in loop.
   * @param conn [const TcpConnectionPtr&]
   */
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

  EventLoop* loop_;                     // the acceptor loop
  const std::string ipPort_;            // listen socket ip:port
  const std::string name_;              // TcpSever name
  std::unique_ptr<Acceptor> acceptor_;  // avoid revealing Acceptor
  std::shared_ptr<EventLoopThreadPool> threadPool_;
  ConnectionCallback connectionCallback_;  // after new connection callback
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  ThreadInitCallback threadInitCallback_;
  AtomicInt32 started_;
  // always in loop thread
  int nextConnId_;
  ConnectionMap connections_;
};
}  // namespace net
}  // namespace mengsen

#endif  // __MENGSEN_NET_TCP_SERVER_H__