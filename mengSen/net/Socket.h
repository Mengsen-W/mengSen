/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-21 17:18:05
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-21 18:23:09
 */

#ifndef __MENGSEN_NET_SOCKET_H__
#define __MENGSEN_NET_SOCKET_H__

#include "../base/noncopyable.h"

struct tcp_info;

namespace mengsen {

namespace net {

class InetAddress;

class Socket : noncopyable {
 public:
  explicit Socket(int sockfd) : sockfd_(sockfd) {}
  Socket(Socket&& s) : sockfd_(s.sockfd_) {}
  ~Socket() {}

  int fd() const { return sockfd_; }
  bool tcp_info(struct tcp_info*) const;
  bool tcp_infoString(char* buf, int len) const;

  void bindAddress(const InetAddress& localaddr);
  void listen();
  int accept(InetAddress* peeraddr);
  void shutdownWrite();

  void setTcpNoDelay(bool on);
  void setReuseAddr(bool on);
  void setReusePort(bool on);
  void setKeepAlive(bool on);

 private:
  const int sockfd_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_SOCKET_H__