/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-07 18:03:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-07 18:45:55
 */

#ifndef __MENGSEN_BUFFER_H__
#define __MENGSEN_BUFFER_H__

#include <cassert>
#include <string>
#include <vector>

namespace mengsen_buffer {

class Buffer {
 public:
  static const size_t kCheapPrepend = 0;
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

 private:
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  static const char kCRLF[];
};

}  // namespace mengsen_buffer

#endif  //__MENGSEN_BUFFER_H__