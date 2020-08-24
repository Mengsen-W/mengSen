/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-23 18:15:03
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-24 21:32:01
 */

#ifndef __MENGSEN_NET_INETADDRESS_H__
#define __MENGSEN_NET_INETADDRESS_H__

#include <netinet/in.h>

#include "../base/StringPiece.h"
#include "../base/copyable.h"

namespace mengsen {

namespace net {

namespace sockets {

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);

}

class InetAddress : copyable {
 public:
  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false,
                       bool ipv6 = false);
  explicit InetAddress(const struct sockaddr_in& addr) : addr_(addr) {}
  explicit InetAddress(const struct sockaddr_in6& addr) : addr6_(addr) {}
  InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

  sa_family_t family() const { return addr_.sin_family; }
  std::string toIp() const;
  std::string toIpPort() const;
  uint16_t toPort() const;

  const struct sockaddr* getSockAddr() const {
    return sockets::sockaddr_cast(&addr6_);
  }

  void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }
  uint32_t ipNetEndian() const;
  uint16_t portNetEndian() const { return addr_.sin_port; }
  static bool resolve(StringArg hostname, InetAddress* result);
  void setScopeId(uint32_t scope_id);

 private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_INETADDRESS_H__