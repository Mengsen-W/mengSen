/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-14 18:26:38
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-14 18:45:15
 */

#ifndef __MENGSEN_CURRENTTHREAD_H__
#define __MENGSEN_CURRENTTHREAD_H__

#include <string>

namespace mengsen {

namespace CurrentThread {

extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char* t_threadName;

void cacheTid();

inline int tid() {
  // UNLIKELY t_cachedTid == 0
  // LIKELY goto return, because if(t_cachedTid == 0)
  if (__builtin_expect(t_cachedTid == 0, 0)) cacheTid();
  return t_cachedTid;
}

inline const char* tidString() { return t_tidString; }

inline int tidStringLength() { return t_tidStringLength; }

inline const char* name() { return t_threadName; }

bool isMainThread();

void sleepUsec(int64_t usec);

std::string stackTrace(bool demangle);
}  // namespace CurrentThread

}  // namespace mengsen

#endif  // __MENGSEN_CURRENTTHREAD_H__
