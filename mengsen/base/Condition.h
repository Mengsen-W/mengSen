/*
 * @Date: 2022-02-21 09:49:59
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-21 09:50:00
 * @FilePath: /mengsen/mengsen/base/Condition.h
 * @Description: file content
 */

// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef __MENGSEN_BASE_CONDITION_H__
#define __MENGSEN_BASE_CONDITION_H__

#include <pthread.h>

#include "mengsen/base/Mutex.h"

namespace mengsen {

class Condition : NonCopyable {
 public:
  explicit Condition(MutexLock& mutex) : mutex_(mutex) {
    MCHECK(pthread_cond_init(&pcond_, NULL));
  }

  ~Condition() { MCHECK(pthread_cond_destroy(&pcond_)); }

  void wait() {
    MutexLock::UnassignGuard ug(mutex_);
    MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()));
  }

  // returns true if time out, false otherwise.
  bool waitForSeconds(double seconds);

  void notify() { MCHECK(pthread_cond_signal(&pcond_)); }

  void notifyAll() { MCHECK(pthread_cond_broadcast(&pcond_)); }

 private:
  MutexLock& mutex_;
  pthread_cond_t pcond_;
};

}  // namespace mengsen

#endif  // __MENGSEN_BASE_CONDITION_H__