/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-19 09:34:31
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-19 12:09:38
 * @Description: proxy string view pass in either a "const char*" or a "string".
 */

#ifndef __MENGSEN_TYPES_STRINGPIECE_H__
#define __MENGSEN_TYPES_STRINGPIECE_H__

#include <cstring>  // for strlen()
#include <iosfwd>

#include "Types.h"

namespace mengSen {

// for passing C-style string arguments to a function
class StringArg {
 public:
  StringArg(const char* str) : str_(str) {}
  StringArg(const std::string& str) : str_(str.c_str()) {}

  // unified API
  const char* c_str() const { return str_; }

 private:
  const char* str_;
};  // end class StringArg

// string view
class StringPiece {
 private:
  const char* ptr_;
  int length_;

 public:
  /**
   * --------------------------------------------------
   * we provide non-explict singleton constructors
   * so users can pass in a "const char*" or a "string"
   * wherever a "StringPiece" is expected
   * --------------------------------------------------
   */

  // constructor
  // null argument constructor
  StringPiece() : ptr_(nullptr), length_(0) {}
  // const char* str constructor
  StringPiece(const char* str)
      : ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}
  // const unsigned char* str constructor
  StringPiece(const unsigned char* str)
      : ptr_(reinterpret_cast<const char*>(str)),
        length_(static_cast<int>(strlen(ptr_))) {}
  // string str constructor
  // use str.size() Omitted step calculation
  StringPiece(const std::string& str)
      : ptr_(str.data()), length_(static_cast<int>(str.size())) {}
  // pointer and length constructor
  StringPiece(const char* offset, int len) : ptr_(offset), length_(len) {}

  // data API
  const char* data() const { return ptr_; }
  int size() const { return length_; }
  bool empty() const { return length_ == 0; }
  const char* begin() const { return ptr_; }
  const char* end() const { return ptr_ + length_; }

  // set & clear API
  void clear() {
    ptr_ = nullptr;
    length_ = 0;
  }
  void set(const char* buffer, int len) {
    ptr_ = buffer;
    length_ = len;
  }
  void set(const char* buffer) {
    ptr_ = buffer;
    length_ = static_cast<int>(strlen(buffer));
  }
  void set(const void* buffer, int len) {
    ptr_ = reinterpret_cast<const char*>(buffer);
    length_ = len;
  }

  // operator[]
  char operator[](int i) { return ptr_[i]; }

  // remove but don't touch raw pointer but modify private ptr_ and length_
  void remove_prefix(int n) {
    ptr_ += n;
    length_ -= n;
  }
  void remove_suffix(int n) { length_ -= n; }

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)                            \
  bool operator cmp(const StringPiece& x) const {                            \
    int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_); \
    return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));          \
  }

  STRINGPIECE_BINARY_PREDICATE(<, <);
  STRINGPIECE_BINARY_PREDICATE(<=, <);
  STRINGPIECE_BINARY_PREDICATE(>=, >) /*;*/
  STRINGPIECE_BINARY_PREDICATE(>, >)  /*;*/
#undef STRINGPIECE_BINARY_PREDICATE

  int compare(const StringPiece& x) const {
    // object of same class are friend each other
    int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
    if (r == 0) {
      if (length_ < x.length_)
        return -1;
      else if (length_ > x.length_)
        return +1;
    }
    return r;
  }

  std::string as_string() const { return std::string(data(), size()); }

  void CopyToSring(std::string* target) const { target->assign(ptr_, length_); }

  // does "this" start with "x"
  bool starts_with(const StringPiece& x) const {
    return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0));
  }
};  // class StringPiece

}  // namespace mengSen

/**
 * --------------------------------------------------------------
 * Function used to create STL containers that use StringPiece
 * Remember that a StringPiece's lifetime had better be less than
 * that of the underlying string or char*.
 * If it is not, than you cannot safely store a StringPiece
 * into STL container.
 * --------------------------------------------------------------
 */

#ifdef HAVE_TYPE_TRAITS
// This makes vector<StringPiece> really fast for some STL implementations
template <>
struct __type_traits<mengSen::StringPiece> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};
#endif

std::ostream& operator<<(std::ostream& out, const mengSen::StringPiece& piece);

#endif  // __MENGSEN_TYPES_STRINGPIECE_H__