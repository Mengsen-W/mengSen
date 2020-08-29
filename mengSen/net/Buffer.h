/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-07 18:03:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-29 18:41:41
 */

#ifndef __MENGSEN_BUFFER_H__
#define __MENGSEN_BUFFER_H__

#include <algorithm>
#include <cassert>
#include <vector>

#include "../base/StringPiece.h"
#include "../base/Types.h"
#include "../base/copyable.h"
#include "Endian.h"

namespace mengsen {

namespace net {

/**
 * @brief Buffer class in net
 */
class Buffer {
 public:
  // front prependable
  static const size_t kCheapPrepend = 8;
  // initial size
  static const size_t kInitialSize = 1024;

  explicit Buffer(size_t initialSize = kInitialSize)
      : buffer_(initialSize + kCheapPrepend),
        readerIndex_(kCheapPrepend),
        writerIndex_(kCheapPrepend) {
    assert(readableBytes() == 0);
    assert(writeableBytes() == initialSize);
    assert(perpendableBytes() == kCheapPrepend);
  }

  void swap(Buffer& rhs) {
    buffer_.swap(rhs.buffer_);
    std::swap(readerIndex_, rhs.readerIndex_);
    std::swap(writerIndex_, rhs.writerIndex_);
  }

  size_t readableBytes() const { return writerIndex_ - readerIndex_; }
  size_t writeableBytes() const { return buffer_.size() - writerIndex_; }
  size_t perpendableBytes() const { return readerIndex_; }
  const char* peek() const { return begin() + readerIndex_; }
  const char* findCRLF() const {
    const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
    crlf == beginWrite() ? nullptr : crlf;
  }

 private:
  char* begin() { return &(*buffer_.begin()); }
  const char* begin() const { return &(*buffer_.begin()); }
  void makeSpace(size_t len) {
    if (writeableBytes() + perpendableBytes() < len + kCheapPrepend) {
      buffer_.resize(writerIndex_ + len);
    } else {
      assert(kCheapPrepend < readerIndex_);
      size_t readable = readableBytes();
      std::copy(begin() + readerIndex_, begin() + writerIndex_,
                begin() + kCheapPrepend);
      readerIndex_ = kCheapPrepend;
      writerIndex_ = readerIndex_ + readable;
      assert(readable == readableBytes());
    }
  }
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  static const char kCRLF[];
};

}  // namespace net

}  // namespace mengsen

#endif  //__MENGSEN_BUFFER_H__