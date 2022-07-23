/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-23 18:12:01
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-23 21:13:13
 */

#include "Socket.h"

#include <netinet/in.h>
#include <netinet/tcp.h>

#include "../base/Logging.h"
#include "InetAddress.h"
#include "SocketsOps.h"

namespace mengsen {

namespace net {

Socket::~Socket() { sockets::close(sockfd_); }

bool Socket::getTcpInfo(struct tcp_info* tcpi) const {
  socklen_t len = sizeof(*tcpi);
  memZero(tcpi, len);
  return ::getsockopt(sockfd_, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getTcpInfoString(char* buf, int len) const {
  struct tcp_info tcpi;
  bool res = getTcpInfo(&tcpi);
  if (res) {
    snprintf(
        buf, len,
        "unrecovered=%u "
        "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
        "lost=%u retrans=%u rtt=%u rttvar=%u "
        "sshthresh=%u cwnd=%u total_retrans=%u",
        tcpi.tcpi_retransmits,     // Number of unrecovered [RTO] timeouts
        tcpi.tcpi_rto,             // Retransmit timeout in usec
        tcpi.tcpi_ato,             // Predicted tick of soft clock in usec
        tcpi.tcpi_snd_mss,         // self MSS
        tcpi.tcpi_rcv_mss,         // other MSS
        tcpi.tcpi_lost,            // Lost packets
        tcpi.tcpi_retrans,         // Retransmitted packets out
        tcpi.tcpi_rtt,             // Smoothed round trip time in usec
        tcpi.tcpi_rttvar,          // Medium deviation
        tcpi.tcpi_snd_ssthresh,    // Slow start threshold
        tcpi.tcpi_snd_cwnd,        // Congestion window
        tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
  }
  return res;
}

void Socket::bindAddress(const InetAddress& addr) {
  sockets::bindOrDie(sockfd_, addr.getSockAddr());
}

void Socket::listen() { sockets::listenOrDie(sockfd_); }

int Socket::accept(InetAddress* peeraddr) {
  struct sockaddr_in6 addr;
  memZero(&addr, sizeof(addr));
  int connfd = sockets::accept(sockfd_, &addr);
  if (connfd >= 0) {
    peeraddr->setSockAddrInet6(addr);
  }
  return connfd;
}

void Socket::shutdownWrite() { sockets::shutdownWrite(sockfd_); }

void Socket::setTcpNoDelay(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval,
               static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReuseAddr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval,
               static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) {
#ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &optval,
                         static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on) {
    LOG_SYSERR << "SO_REUSEPORT failed.";
  }
#else
  if (on) {
    LOG_ERROR << "SO_REUSEPORT is not supported.";
  }
#endif
}

void Socket::setKeepAlive(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &optval,
               static_cast<socklen_t>(sizeof(optval)));
}

}  // namespace net

}  // namespace mengsen