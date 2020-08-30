/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-07 18:03:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-29 22:06:08
 */

#ifndef __MENGSEN_NET_BUFFER_H__
#define __MENGSEN_NET_BUFFER_H__

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
 * @brief perpend --- readerIndex_ --- writerIndex
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
    assert(prependableBytes() == kCheapPrepend);
  }

  /**
   * @brief swap buffers
   */
  void swap(Buffer& rhs) {
    buffer_.swap(rhs.buffer_);
    std::swap(readerIndex_, rhs.readerIndex_);
    std::swap(writerIndex_, rhs.writerIndex_);
  }

  /**
   * @return readable bytes number
   */
  size_t readableBytes() const { return writerIndex_ - readerIndex_; }

  /**
   * @return writeable bytes number
   */
  size_t writeableBytes() const { return buffer_.size() - writerIndex_; }

  /**
   * @return perpendable bytes number
   */
  size_t prependableBytes() const { return readerIndex_; }

  /**
   * @brief find kCRLF in read zone of buffer
   * @return pointer of set of buffers [const char*]
   */
  const char* findCRLF() const {
    const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
    crlf == beginWrite() ? nullptr : crlf;
  }

  /**
   * @brief find kCRLF in read zone of buffer
   * @param start [const char *]
   * @return pointer of set of buffers [const char*]
   */
  const char* findCRLF(const char* start) const {
    assert(peek() <= start);
    assert(start <= beginWrite());
    const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
    return crlf == beginWrite() ? nullptr : crlf;
  }

  /**
   * @brief find EOF in read zone of buffer
   * @return pointer of set of buffers [const char*]
   */
  const char* findEOL() const {
    const void* eol = memchr(peek(), '\n', readableBytes());
    return static_cast<const char*>(eol);
  }

  /**
   * @brief find EOF in read zone of buffer
   * @param start [const char *]
   * @return pointer of set of buffers [const char*]
   */
  const char* findEOL(const char* start) const {
    assert(peek() <= start);
    assert(start <= beginWrite());
    const void* eol = memchr(start, '\n', beginWrite() - start);
  }

  /**
   * @brief retrieve all of buffer
   */
  void retrieveAll() {
    readerIndex_ = kCheapPrepend;
    writerIndex_ = kCheapPrepend;
  }

  /**
   * @brief retrieve form reader index to length
   * @param len [size_t]
   */
  void retrieve(size_t len) {
    assert(len <= readableBytes());
    if (len < readableBytes()) {
      readerIndex_ += len;
    } else {
      // len == readableBytes()
      retrieveAll();
    }
  }

  /**
   * @brief retrieve reader index to end
   * @param end [const char*]
   * @return void
   */
  void retrieveUntil(const char* end) {
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
  }

  void retrieveInt64() { retrieve(sizeof(int64_t)); }

  void retrieveInt32() { retrieve(sizeof(int32_t)); }

  void retrieveInt16() { retrieve(sizeof(int16_t)); }

  void retrieveInt8() { retrieve(sizeof(int8_t)); }

  /**
   * @brief retrieve all of reader of buffer to string
   * @return string
   */
  std::string retrieveAllString() { return retrieveAsString(readableBytes()); }

  /**
   * @brief retrieve and string with read index to length
   * @param len [size_t]
   * @return string
   */
  std::string retrieveAsString(size_t len) {
    assert(len <= readableBytes());
    std::string result(peek(), len);
    retrieve(len);
    return result;
  }

  /**
   * @brief StringPiece with read index
   * @return StringPiece
   */
  StringPiece toStringPiece() const {
    return StringPiece(peek(), static_cast<int>(readableBytes()));
  }

  void append(const StringPiece& str) { append(str.data(), str.size()); }

  void append(const char* data, size_t len) {
    ensureWritableBytes(len);
    std::copy(data, data + len, beginWrite());
    hasWritten(len);
  }

  void append(const void* data, size_t len) {
    append(static_cast<const char*>(data), len);
  }

  void ensureWritableBytes(size_t len) {
    if (writeableBytes() < len) {
      makeSpace(len);
    }
    assert(writeableBytes() >= len);
  }

  char* beginWrite() { return begin() + writerIndex_; }

  const char* beginWrite() const { return begin() + writerIndex_; }

  void hasWritten(size_t len) {
    assert(len <= writeableBytes());
    writerIndex_ += len;
  }

  void unwrite(size_t len) {
    assert(len <= readableBytes());
    writerIndex_ -= len;
  }

  void appendInt64(int64_t x) {
    int64_t be64 = sockets::hostToNetwork64(x);
    append(&be64, sizeof(be64));
  }

  void appendInt32(int32_t x) {
    int32_t be32 = sockets::hostToNetwork32(x);
    append(&be32, sizeof(be32));
  }

  void appendInt16(int16_t x) {
    int16_t be16 = sockets::hostToNetwork16(x);
    append(&be16, sizeof(be16));
  }

  void appendInt8(int8_t x) { append(&x, sizeof(x)); }

  int64_t readInt64() {
    int64_t result = peekInt64();
    retrieveInt64();
    return result;
  }

  int32_t readInt32() {
    int32_t result = peekInt32();
    retrieveInt32();
    return result;
  }

  int16_t readInt16() {
    int16_t result = peekInt16();
    retrieveInt16();
    return result;
  }

  int8_t readInt8() {
    int8_t result = peekInt8();
    retrieveInt8();
    return result;
  }

  /**
   * @return reader index pointers [const char *]
   */
  const char* peek() const { return begin() + readerIndex_; }

  int64_t peekInt64() const {
    assert(readableBytes() >= sizeof(int64_t));
    int64_t be64 = 0;
    ::memcpy(&be64, peek(), sizeof(be64));
    return sockets::networkToHost64(be64);
  }

  int32_t peekInt32() const {
    assert(readableBytes() >= sizeof(int32_t));
    int32_t be32 = 0;
    ::memcpy(&be32, peek(), sizeof(be32));
    return sockets::networkToHost32(be32);
  }

  int16_t peekInt16() const {
    assert(readableBytes() >= sizeof(int16_t));
    int16_t be16 = 0;
    ::memcpy(&be16, peek(), sizeof(be16));
    return sockets::networkToHost16(be16);
  }

  int8_t peekInt8() const {
    assert(readableBytes() >= sizeof(int8_t));
    int8_t x = *peek();
    return x;
  }

  void perpendInt64(int64_t x) {
    int64_t be64 = sockets::hostToNetwork64(x);
    prepend(&be64, sizeof be64);
  }

  void perpendInt32(int32_t x) {
    int32_t be32 = sockets::hostToNetwork32(x);
    prepend(&be32, sizeof be32);
  }

  void perpendInt16(int16_t x) {
    int16_t be16 = sockets::hostToNetwork16(x);
    prepend(&be16, sizeof be16);
  }

  void perpendInt8(int8_t x) { prepend(&x, sizeof(x)); }

  void prepend(const void* data, size_t len) {
    assert(len <= prependableBytes());
    readerIndex_ -= len;
    const char* d = static_cast<const char*>(data);
    std::copy(d, d + len, begin() + readerIndex_);
  }

  void shrink(size_t reserve) {
    // FIXME: use vector::shrink_to_fit() in C++ 11 if possible.
    Buffer other;
    other.ensureWritableBytes(readableBytes() + reserve);
    other.append(toStringPiece());
    swap(other);
  }

  size_t internalCapacity() const { return buffer_.capacity(); }

 private:
  char* begin() { return &(*buffer_.begin()); }

  const char* begin() const { return &(*buffer_.begin()); }

  void makeSpace(size_t len) {
    if (writeableBytes() + prependableBytes() < len + kCheapPrepend) {
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

 private:
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  static const char kCRLF[];
};

}  // namespace net

}  // namespace mengsen

#endif  //__MENGSEN_NET_BUFFER_H__