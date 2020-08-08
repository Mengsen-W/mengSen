/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 16:27:55
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 18:43:45
 */

#ifndef __MENGSEN_CURRENTTHREAD_H__
#define __MENGSEN_CURRENTTHREAD_H__

#include <string>
namespace mengsen {

namespace CurrentThread {
// cache pid_t
extern thread_local int t_cachedTid;
// cache pthread pid_t In the form of a String
extern thread_local char t_tidString[32];
// cache pthread t_tidString length
extern thread_local int t_tidStringLength;
// cache pthread name
extern thread_local const char* t_threadName;

/**
 * @brief cache Tid in t_cachedTid
 * @param void
 * @return [void]
 */
void cacheTid();

/**
 * @brief [inline] return t_cachedTid
 * @param void
 * @return [int]
 */
inline int tid() {
  if (__builtin_expect(t_cachedTid == 0, 0)) cacheTid();
  return t_cachedTid;
}

/**
 * @brief [inline] return t_tidString
 * @param void
 * @return [const char*]
 */
inline const char* tidString() { return t_tidString; }

/**
 * @brief [inline] return t_tidStringLength
 * @param void
 * @return [int]
 */
inline int tidStringLength() { return t_tidStringLength; }

/**
 * @brief [inline] return t_threadName
 * @param void
 * @return [const char*]
 */
inline const char* name() { return t_threadName; }

/**
 * @brief Determine if it is the main thread of call this function thread
 * @param void
 * @return [bool]
 */
bool isMainThread();

/**
 * @brief sleep microseconds
 * @param usec [int64_t]
 * @return [void]
 */
void sleepUsec(int64_t usec);

/**
 * @brief return stack trace
 * @param demangle [bool] Convert to readable, default true
 * @return [std::string]
 */
std::string stackTrace(bool demangle = true);

}  // namespace CurrentThread

}  // namespace mengsen

#endif