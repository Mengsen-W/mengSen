/*
 * @Date: 2022-02-21 09:51:06
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-21 10:16:54
 */

#include "mengsen/base/Condition.h"

#include <cerrno>

// returns true if time out, false otherwise.
namespace mengsen {

bool Condition::waitForSeconds(double seconds) {
  struct timespec abstime;
  // FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
  clock_gettime(CLOCK_REALTIME, &abstime);

  const int64_t kNanoSecondsPerSecond = 1000000000;
  int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

  abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) /
                                        kNanoSecondsPerSecond);
  abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) %
                                      kNanoSecondsPerSecond);

  MutexLock::UnassignGuard ug(mutex_);
  return ETIMEDOUT ==
         pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}

}  // namespace mengsen