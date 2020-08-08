/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-08 11:13:26
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-08 11:23:04
 */

#ifndef __MENGSEN_SPINLOCK_H__
#define __MENGSEN_SPINLOCK_H__

#include <atomic>

namespace mengsen {

class SpinLock {
 public:
  SpinLock(std::atomic_flag& flag) : _flag(flag) {
    while (_flag.test_and_set(std::memory_order_acquire))
      ;
  }
  ~SpinLock() { _flag.clear(std::memory_order_release); }

 private:
  std::atomic_flag& _flag;
};

}  // namespace mengsen

#endif  //__MENGSEN_SPINLOCK_H__