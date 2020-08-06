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

// cache pid_t
extern __thread int t_cachedTid;
// cache pthread pid_t In the form of a String
extern __thread char t_tidString[32];
// cache pthread t_tidString length
extern __thread int t_tidStringLength;
// cache pthread name
extern __thread const char* t_threadName;

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
  // #define LIKELY(x) __builtin_expect(!!(x), 1) //x很可能为真
  // #define UNLIKELY(x) __builtin_expect(!!(x), 0) //x很可能为假
  // UNLIKELY t_cachedTid == 0
  // LIKELY goto return, because if(t_cachedTid == 0)
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

#endif  // __MENGSEN_CURRENTTHREAD_H__

/*
pid_t 是一个小整数，直接表示内核的调度id
任何时刻都是全局唯一的，0是非法值
*/
/*
pthread_t 不确定其类型，无法比较或计算其hash值
无法定义一个非法的pthread_t值，只在进程中有意义
*/