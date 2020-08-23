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

int createNonblockingOrDie(sa_family_t family);
int bindOrDie(int sockfd, const struct sockaddr* addr);
int listenOrDie(int sockfd);
int accept(int sockfd, struct sockaddr_in6* addr);
int connent(int sockfd, const struct sockaddr* addr);
void close(int sockfd);
void shutdownWrite(int sockfd);

ssize_t read(int sockfd, void* buf, size_t count);
ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);
ssize_t write(int sockfd, const void* buf, size_t count);

void toIp(char* buf, size_t size, const struct sockaddr* addr);
void toIpPort(char* buf, size_t size, const struct sockaddr* addr);
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in* addr);
void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in6* addr);

int getSocketError(int sockfd);
struct sockaddr_in6 getLocalAddr(int sockfd);
struct sockaddr_in6 getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);

const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in6* addr);
const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);
const struct sockaddr_in6* sockaddr_in6_cast(const struct sockaddr* addr);

}  // namespace sockets

}  // namespace net

}  // namespace mengsen

#endif  // __MENGSEN_NET_SOCKETOPS_H__