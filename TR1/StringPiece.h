/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-09 20:50:41
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-09 20:51:29
 */

#ifndef __MENGSEN_STRINGPIECE_H__
#define __MENGSEN_STRINGPIECE_H__

#include <string.h>

#include <iosfwd>
#include <string>

namespace mengsen {

class StringArg {
 public:
  StringArg(const char* str) : str_(str) {}
  StringArg(const std::string& str) : str_(str.c_str()) {}

  const char* c_str() const { return str_; }

 private:
  const char* str_;
};

class StringPiece {
 private:
  const char* ptr_;
  int length_;

 public:
  StringPiece() : ptr_(nullptr), length_(0) {}
  StringPiece(const char* str)
      : ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}
  StringPiece(const unsigned char* str)
      : ptr_(reinterpret_cast<const char*>(str)),
        length_(static_cast<int>(strlen(ptr_))) {}
  StringPiece(const std::string& str)
      : ptr_(str.data()), length_(static_cast<int>(str.size())) {}
  StringPiece(const char* offset, int len) : ptr_(offset), length_(len) {}

  const char* data() const { return ptr_; }
  std::string as_string() const { return std::string(data(), size()); }
  void CopyToString(std::string* target) const {
    target->assign(ptr_, length_);
  }
  int size() const { return length_; }
  bool empty() const { return length_ == 0; }
  const char* begin() const { return ptr_; }
  const char* end() const { return ptr_ + length_; }

  /**
   * @brief this starts with x
   * @param x [const StringPiece &]
   * @return bool
   */
  bool starts_with(const StringPiece& x) const {
    return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_)));
  }

  void clean() {
    ptr_ = nullptr;
    length_ = 0;
  }
  void set(const char* buffer, int len) {
    ptr_ = buffer;
    length_ = len;
  }
  void set(const char* str) {
    ptr_ = str;
    length_ = static_cast<int>(strlen(str));
  }
  void set(const void* buffer, int len) {
    ptr_ = reinterpret_cast<const char*>(buffer);
    length_ = len;
  }

  char operator[](int i) const { return ptr_[i]; }
  bool operator==(const StringPiece& x) const {
    return ((length_ == x.length_) && (memcmp(ptr_, x.ptr_, length_) == 0));
  }
  bool operator!=(const StringPiece& x) const { return !(*this == x); }

  void remove_prefix(int n) {
    ptr_ += n;
    length_ -= n;
  }
  void remove_suffix(int n) { length_ -= n; }

#define STRINGPIECE_BINARY_PREDICATE(cmp, auxcmp)                            \
  bool operator cmp(const StringPiece x) const {                             \
    int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_); \
    return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));          \
  }

  STRINGPIECE_BINARY_PREDICATE(<, <);
  STRINGPIECE_BINARY_PREDICATE(<=, <);
  STRINGPIECE_BINARY_PREDICATE(>=, >);
  STRINGPIECE_BINARY_PREDICATE(>, >);

#undef STRINGPIECE_BINARY_PREDICATE

  int compare(const StringPiece& x) const {
    int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
    if (r == 0) {
      if (length_ < x.length_)
        r = -1;
      else if (length_ > x.length_)
        r = +1;
    }
    return r;
  }

};  // namespace mengsen

}  // namespace mengsen

// ------------------------------------------------------------------
// Functions used to create STL containers that use StringPiece
//  Remember that a StringPiece's lifetime had better be less than
//  that of the underlying string or char*.  If it is not, then you
//  cannot safely store a StringPiece into an STL container
// ------------------------------------------------------------------

#ifdef HAVE_TYPE_TRAITS

template <>
struct __type_traits<> {
  typedef __true_type has_trivial_default_constructor;
  typedef __true_type has_trivial_copy_constructor;
  typedef __true_type has_trivial_assignment_operator;
  typedef __true_type has_trivial_destructor;
  typedef __true_type is_POD_type;
};

#endif

std::ostream& operator<<(std::ostream& o, mengsen::StringPiece& piece);

#endif  // __MENGSEN_STRINGPIECE_H__