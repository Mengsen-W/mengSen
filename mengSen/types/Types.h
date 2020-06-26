/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-19 08:48:40
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-19 09:16:19
 * @Description: something of type and type convert
 */

#ifndef __MENGSEN_TYPES_TYPES_H__
#define __MENGSEN_TYPES_TYPES_H__

#include <string.h>  // memset

#include <string>  // size_t

#ifndef NOEBUG
#include <cassert>
#endif

namespace mengSen {
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

}  // namespace mengSen

#endif  // __MENGSEN_TYPES_TYPES_H__
