/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-21 16:08:56
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-23 17:58:53
 */

#ifndef __MENGSEN_NET_SOCKETOPS_H__
#define __MENGSEN_NET_SOCKETOPS_H__

#include <arpa/inet.h>

namespace mengsen {

namespace net {

namespace sockets {

/// this socket namespace doesn't appear on the client

/**
 * @brief create a non-blocking family
 * @param family [sa_family_t]
 * @return [int] error number
 */
int createNonblockingOrDie(sa_family_t family);

/**
 * @brief bind sockfd and sockaddr
 * @param sockfd [int]
 * @param addr [const struct sockaddr*]
 * @return [int] error number
 */
int bindOrDie(int sockfd, const struct sockaddr* addr);

/**
 * @brief set listen fd
 * @param sockfd [int]
 * @return [int] error number
 */
int listenOrDie(int sockfd);

/**
 * @brief accept sockfd to sockaddr
 * @param sockfd [int]
 * @param addr [struct sockaddr_in6*]
 * @return [int] error number
 */
int accept(int sockfd, struct sockaddr_in6* addr);

/**
 * @brief connect with input sockfd and output sockaddr
 * @param sockfd [int]
 * @param addr [const struct sockaddr*]
 * @return [int] error number
 */
int connent(int sockfd, const struct sockaddr* addr);

/**
 * @brief close fd
 * @param sockfd [int]
 */
void close(int sockfd);

/**
 * @brief just shutdown this end write
 * @param sockfd [int]
 */
void shutdownWrite(int sockfd);

/**
 * @brief read from input sockfd count bytes to buf
 * @param sockfd [int]
 * @param buf [void *]
 * @param count [size_t]
 * @return [ssize_t] actually read bytes number
 */
ssize_t read(int sockfd, void* buf, size_t count);

/**
 * @brief read from input sockfd iovcnt bytes to iovec
 * @param sockfd [int]
 * @param iov [const struct iovec *]
 * @param iovcnt [int]
 * @return [ssize_t] actually read bytes number
 */
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);

/**
 * @brief write count bytes from buf to sockfd
 * @param sockfd [int]
 * @param buf [const void *]
 * @param count [size_t]
 * @return [ssize_t] actually write bytes number
 */
ssize_t write(int sockfd, const void* buf, size_t count);

/**
 * @brief convert net ip addr to Dot the decimal system from addr to buf
 * @param buf [char *] save decimal
 * @param size [size_t] size of buffer
 * @param addr [const struct sockaddr*] source addr
 */
void toIp(char* buf, size_t size, const struct sockaddr* addr);

/**
 * @brief convert net ip and port to Dot the decimal system from addr to buf
 * @param buf [void *] save decimal
 * @param size [size_t] size of buffer
 * @param addr [const struct sockaddr*]
 */
void toIpPort(char* buf, size_t size, const struct sockaddr* addr);

/**
 * @brief convert Dot the decimal system from ip and port to sockaddr_in
 * @param ip [const char*]
 * @param port [uint16_t]
 * @param addr [sockaddr_in*]
 */
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

/**
 * @brief convert Dot the decimal system from ip and port to sockaddr_in6
 * @param ip [const char*]
 * @param port [uint16_t]
 * @param addr [sockaddr_in6*]
 */
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

/**
 * @brief get socket error with input sockfd
 * @param sockfd [int]
 * @return [int] error number
 */
int getSocketError(int sockfd);

/**
 * @brief get local addr with local sockfd
 * @param sockfd [int]
 * @return [struct sockaddr_in6]
 */
struct sockaddr_in6 getLocalAddr(int sockfd);

/**
 * @brief get peer addr with local sockfd
 * @param sockfd [int]
 * @return [struct sockaddr_in6]
 */
struct sockaddr_in6 getPeerAddr(int sockfd);

/**
 * @brief check localaddr and peeraddr is same
 * @param sockfd [int] local sockfd
 * @return [bool]
 */
bool isSelfConnect(int sockfd);

/**
 * @brief convert input param to return param
 * @param addr [const struct sockaddr_in6*]
 * @return [const struct sockaddr*]
 */
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);

/**
 * @brief convert input param to return param
 * @param addr [struct sockaddr_in6*]
 * @return [struct sockaddr*]
 */
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);

/**
 * @brief convert input param to return param
 * @param addr [const struct sockaddr_in*]
 * @return [const struct sockaddr*]
 */
const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);

/**
 * @brief convert input param to return param
 * @param addr [struct sockaddr_in*]
 * @return [struct sockaddr*]
 */
struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);

/**
 * @brief convert input param to return param
 * @param addr [struct sockaddr*]
 * @return [struct sockaddr_in*]
 */
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);

/**
 * @brief convert input param to return param
 * @param addr [const struct sockaddr*]
 * @return [const struct sockaddr_in*]
 */
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

}  // namespace sockets

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_SOCKETOPS_H__