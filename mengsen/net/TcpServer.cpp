/*
 * @Date: 2022-03-04 23:16:32
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-07 20:03:40
 * @FilePath: /mengsen/mengsen/net/TcpServer.cpp
 */

#include "mengsen/net/TcpServer.h"

#include "mengsen/base/Logging.h"
#include "mengsen/net/Acceptor.h"
#include "mengsen/net/EventLoop.h"
#include "mengsen/net/EventLoopThreadPool.h"
#include "mengsen/net/Socket.h"
#include "mengsen/net/SocketsOps.h"

namespace mengsen {
namespace net {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr,
                     const std::string& nameArg, Option option)
    : loop_(CHECK_NOTNULL(loop)),
      ipPort_(listenAddr.toIpPort()),
      name_(nameArg),
      acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
      threadPool_(new EventLoopThreadPool(loop, name_)),
      connectionCallback_(defaultConnectionCallback),
      messageCallback_(defaultMessageCallback),
      nextConnId_(1) {
  acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,
                                                std::placeholders::_1,
                                                std::placeholders::_2));
}

TcpServer::~TcpServer() {
  loop_->assertInLoopThread();
  LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

  for (auto& item : connections_) {
    TcpConnectionPtr conn(item.second);
    // destruct shared_ptr
    item.second.reset();
    // destruct TcpConnection in that loop thread
    conn->getLoop()->runInLoop(
        std::bind(&TcpConnection::connectDestroyed, conn));
  }
}

void TcpServer::setThreadNum(int numThreads) {
  assert(0 <= numThreads);
  threadPool_->setThreadNum(numThreads);
}

void TcpServer::start() {
  if (started_.getAndSet(1) == 0) {  // thread safe
    threadPool_->start(threadInitCallback_);
    assert(!acceptor_->listening());  // not listening yet
    loop_->runInLoop(std::bind(&Acceptor::listen, get_pointer(acceptor_)));
  }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
  loop_->assertInLoopThread();
  EventLoop* ioLoop = threadPool_->getNextLoop();
  char buf[64];
  snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
  ++nextConnId_;
  std::string connName = name_ + buf;

  LOG_INFO << "TcpServer::newConnection [" << name_ << "] - new connection ["
           << connName << "] from " << peerAddr.toIpPort();
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection, that
  // sockfd == -1
  //  use make_shared construct once, new and convert shared_ptr
  // constructor once object and constructor once shared_ptr control object =>
  // (new object) + (new shared_ptr reference count)
  TcpConnectionPtr conn{std::make_shared<TcpConnection>(
      ioLoop, connName, sockfd, localAddr, peerAddr)};
  connections_[connName] = conn;
  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(
      std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
  // FIXME: unsafe
  ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
  // FIXME: unsafe
  loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
  loop_->assertInLoopThread();
  LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
           << "] - connection " << conn->name();
  size_t n = connections_.erase(conn->name());
  assert(n == 1);
  EventLoop* ioLoop = conn->getLoop();
  ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

}  // namespace net

}  // namespace mengsen