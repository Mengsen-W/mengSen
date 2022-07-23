/*
 * @Date: 2020-08-21 17:18:05
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 08:38:36
 * @FilePath: /mengsen/mengsen/net/Socket.h
 * @Description: file content
 */

#ifndef __MENGSEN_NET_SOCKET_H__
#define __MENGSEN_NET_SOCKET_H__

#include <utility>  // std::move

#include "mengsen/base/NonCopyable.h"

struct tcp_info;

namespace mengsen {

namespace net {

class InetAddress;

/**
 * @brief socket class
 * @param sockfd [int]
 */
class Socket : NonCopyable {
 public:
  explicit Socket(int sockfd) : sockfd_(sockfd) {}
  explicit Socket(Socket&& s) : sockfd_(std::move(s.sockfd_)) {}
  ~Socket();

  /**
   * @brief get fd
   * @return [int]
   */
  int fd() const { return sockfd_; }

  /**
   * @brief from tcp_info* to get Tcp info
   * @param tcp_info* [in]
   * @return [bool] for success
   */
  bool getTcpInfo(struct tcp_info*) const;

  /**
   * @brief through to para buf tcp info
   * @param buf [char *] [in]
   * @param len [int] [in]
   * @return [bool]
   */
  bool getTcpInfoString(char* buf, int len) const;

  /**
   * @brief bind whit localaddr
   * @param localaddr [const InetAddress&]
   */
  void bindAddress(const InetAddress& localaddr);

  /**
   * @brief listen this socket
   */
  void listen();

  /**
   * @brief accept peeraddr
   * @param peeraddr [InetAddress*] [in]
   * @return [int] connect fd
   */
  int accept(InetAddress* peeraddr);

  /**
   * @brief shutdown this end write
   */
  void shutdownWrite();

  /**
   * @brief set TcpNODelay for no Nagle
   * @param on [bool]
   */
  void setTcpNoDelay(bool on);
  /**
   * @brief set reuse addr
   * @param on [bool]
   */
  void setReuseAddr(bool on);
  /**
   * @brief set reuse port
   * @param on [bool]
   */
  void setReusePort(bool on);
  /**
   * @brief set keep alive
   * @param on [bool]
   */
  void setKeepAlive(bool on);

 private:
  // mangered sockfd
  const int sockfd_;
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_SOCKET_H__