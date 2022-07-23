/*
 * @Date: 2020-05-19 08:48:40
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:28:57
 * @FilePath: /mengsen/mengsen/base/Types.h
 */

#ifndef __MENGSEN_TYPES_H__
#define __MENGSEN_TYPES_H__

#include <cstring>  // memset
#include <string>   // size_t

#ifndef NODEBUG
#include <cassert>
#endif

namespace mengsen {

inline void memZero(void *p, size_t n) { memset(p, 0, n); }

// a safe version to upcast
template <typename To, typename From>
inline To implicit_cast(From const &f) {
  return f;
}

// a safe version to downcast
template <typename To, typename From>
inline To downcast(From *f) {
  if (false) {
    implicit_cast<From *, To>(0);
  }
#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
  // RTTI: debug mode only
  assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
  return static_cast<To>(f);
}

}  // namespace mengsen

#endif  // __MENGSEN_TYPES_H__
