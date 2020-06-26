/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-19 08:20:28
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-20 22:13:22
 * @Description: format log to stream
 */

#ifndef __MENGSEN_LOG_LOGSTREAM_H__
#define __MENGSEN_LOG_LOGSTREAM_H__

#include <cassert>
#include <string>

#include "../types/StringPiece.h"  // for StringPiece
#include "../types/Types.h"        // for implicit_cast
#include "../types/noncopyable.h"  // for non-copyable

namespace mengSen {

namespace detail {

// buffer size
const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer : noncopyable {
 public:
  FixedBuffer() {}
  ~FixedBuffer() {}

  void append(const char* buf, size_t len) {
    if (implicit_cast<size_t>(avail() > len)) {
      memcpy(cur_, buf, len);
      cur += len;
    }
  }

  const char* data() const { return data_; }
  int length() const { return static_cast<int>(cur_ - data_); }

  char* current() const { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(size_t len) { cur += len; }

  void reset() { cur_ = data_; }
  void bzero() { memZero(data_, sizeof(data_)); }

  // for used by GDB
  const char* debugString();
  void setCookie(void (*cookie)()) { cookie_ = cookie; }

  // for used by unit test
  std::string toString() const { return std::string(data_, length()); }
  StringPiece toStringPiece() const { return StringPiece(data_, length()); }

 private:
  const char* end() const { return data_ + sizeof(data_); }

  // cookie function must be outline
  static void cookieStart();
  static void cookieEnd();

  void (*cookie_)();  // function pointer for cookie
  char data_[SIZE];   // head pointer
  char* cur_;         // next pointer
};

}  // namespace detail

class LogStream : noncopyable {
  typedef LogStream self;

 public:
  typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

  self& operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  // operator <<
  self& operator<<(short);
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);

  self& operator<<(const void*);

  self& operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
  }

  self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v) {
    buffer_.append(&v, 1);
    return *this;
  }
  // self& operator<<(signed char);
  // self& operator<<(unsigned char);
  self& operator<<(const char* str) {
    if (str) {
      buffer_.append(str, strlen(str));
    } else {
      buffer_.append("(null)", 6);
    }
    return *this;
  }

  self& operator<<(const unsigned char* str) {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  self& operator<<(const std::string& v) {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  self& operator<<(const StringPiece& v) {
    buffer_.append(v.data(), v.size());
    return *this;
  }

  self& operator<<(const Buffer& v) { return operator<<(v.toStringPiece()); }

  void append(const char* data, int len) { buffer_.append(data, len); }

  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }

 private:
  void staticCheck();

  template <typename T>
  void formatInteger(T);

  Buffer buffer_;

  // static const must be init whit this way
  static const int kMaxNumbericSize = 32;
};

class Fmt {
 public:
  template <typename T>
  Fmt(const char* fmt, T val);

  const char* data() const { return buf_; }
  int length() const { return length_; }

 private:
  char buf_[32];
  int length_;
};

// Format quantity n in SI units (k, M, G, T, P, E)
// The required string is atmost 5 characters long
// requires n >= 0
std::string formatSI(int64_t n);

// Format quantity n in IEC units (Ki, Mi, Gi, Ti, Pi, Ei)
// The required string is atmost 6 characters long
// requires n >= 0
std::string formatSI(int64_t n);

}  // namespace mengSen
#endif  // __MENGSEN_LOG_LOGSTREAM_H__
