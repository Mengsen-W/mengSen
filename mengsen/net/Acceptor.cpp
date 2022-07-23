/*
 * @Date: 2022-01-13 10:41:56
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 10:14:35
 * @FilePath: /mengsen/mengsen/net/Acceptor.cpp
 */

#include "mengsen/net/Acceptor.h"

#include <fcntl.h>  // ::open()

#include <cassert>

#include "mengsen/base/Logging.h"
#include "mengsen/net/EventLoop.h"    // loop_->assertInLoopThread()()
#include "mengsen/net/InetAddress.h"  // listenAddr.family()
#include "mengsen/net/SocketsOps.h"   // sockets::createNonblockingOrDie()

using namespace mengsen::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr,
                   bool reuseport)
    : loop_(loop),
      acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),
      acceptChannel_(loop, acceptSocket_.fd()),
      listening_(false),
      // init idleFd_
      idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {
  assert(idleFd_ >= 0);
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.setReusePort(reuseport);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
  acceptChannel_.disableAll();
  acceptChannel_.remove();
  ::close(idleFd_);
}

void Acceptor::listen() {
  loop_->assertInLoopThread();
  listening_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
  loop_->assertInLoopThread();
  InetAddress peerAddr;
  // FIXME loop until no more
  int connfd = acceptSocket_.accept(&peerAddr);
  if (connfd >= 0) {
    std::string hostport = peerAddr.toIpPort();
    LOG_TRACE << "Accepts of " << hostport;
    if (newConnectionCallback_) {
      newConnectionCallback_(connfd, peerAddr);
    } else {
      sockets::close(connfd);
    }
  } else {
    LOG_SYSERR << "in Acceptor::handleRead";
    // Read the section named "The special problem of
    // accept()ing when you can't" in libev's doc.
    // By Marc Lehmann, author of libev.
    if (errno == EMFILE) {
      ::close(idleFd_);
      idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
      ::close(idleFd_);
      idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    }
  }
}
