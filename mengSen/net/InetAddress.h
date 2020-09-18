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

/**
 * @brief convert sockaddr_in6 to sockaddr, and do not include "SocketsOps.h"
 * @param addr [const struct sockaddr_in6*]
 * @return [const struct sockaddr*]
 */
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);

}  // namespace sockets

/**
 * @brief wrapper of sockaddr_in use this class in listen()/accept() etc. or Socket class
 */
class InetAddress : copyable {
 public:
  explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false,
                       bool ipv6 = false);
  explicit InetAddress(const struct sockaddr_in& addr) : addr_(addr) {}
  explicit InetAddress(const struct sockaddr_in6& addr) : addr6_(addr) {}
  InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

  /**
   * @brief get sa_family_t
   * @return sa_family_t
   */
  sa_family_t family() const { return addr_.sin_family; }

  /**
   * @brief get ip with std::string type
   * @return std::string
   */
  std::string toIp() const;

  /**
   * @brief get port with std::string type
   * @return std::string
   */
  std::string toIpPort() const;

  /**
   * @brief get port whit uint16_t
   * @return uint16_t
   */
  uint16_t toPort() const;

  /**
   * @brief get sockaddr*
   * @return const struct sockaddr*
   */
  const struct sockaddr* getSockAddr() const {
    // just for Cooperate to define
    // Actually the same address on addr6_ and addr_
    return sockets::sockaddr_cast(&addr6_);
  }

  /**
   * @brief set sockaddr whit param
   * @param addr6 [const struct sockaddr_in6&]
   */
  void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }

  /**
   * @brief get ip with NetEndian
   * @return uint32_t
   */
  uint32_t ipNetEndian() const;

  /**
   * @brief get port with NetEndian
   * @return uint16_t
   */
  uint16_t portNetEndian() const { return addr_.sin_port; }

  /**
   * @brief resolve hostname to IP address, not changing port or sin_family,
   * thread safe
   * @return true on success
   */
  static bool resolve(StringArg hostname, InetAddress* result);

  /**
   * @brief set IPv6 ScopeID
   */
  void setScopeId(uint32_t scope_id);

 private:
  // save ipv4 or ipv6
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_INETADDRESS_H__