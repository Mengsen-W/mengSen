/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-13 21:24:37
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-13 21:33:11
 */

#include "Thread.h"

#include <error.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

namespace mengsen {

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

}  // namespace mengsen
