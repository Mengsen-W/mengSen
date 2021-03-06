/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-29 18:21:54
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-29 18:35:58
 */

#include "Buffer.h"

#include <error.h>
#include <sys/uio.h>

#include "SocketsOps.h"

namespace mengsen {

namespace net {

const size_t Buffer::kCheapPrepend;
const size_t Buffer::kInitialSize;
const char Buffer::kCRLF[] = "\r\n";

ssize_t Buffer::readFd(int fd, int* savedErrno) {
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin() + writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof(extrabuf);
  // when there is enough space in this buffer, don't read into extrabuf.
  // when extrabuf is used, we read 128k-1 bytes at most.
  const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
  const ssize_t n = sockets::readv(fd, vec, iovcnt);
  if (n < 0) {
    *savedErrno = errno;

  } else if (implicit_cast<size_t>(n) <= writable) {
    writerIndex_ += n;
  } else {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  return n;
}

}  // namespace net

}  // namespace mengsen