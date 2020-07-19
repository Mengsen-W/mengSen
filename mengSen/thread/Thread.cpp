/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-13 21:24:37
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-14 18:23:23
 */

#include "Thread.h"

#include <error.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "CurrentThread.h"

namespace mengsen {

namespace detail {

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void afterFork() {
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "main";
  CurrentThread::tid();
}

class ThreadNameInitializer {
 public:
  ThreadNameInitializer() {
    CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};
// FIXME :scope of all
ThreadNameInitializer init;

struct ThreadDate {};

}  // namespace detail

}  // namespace mengsen
