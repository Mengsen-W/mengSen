/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-09 20:21:00
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 22:34:28
 */

#ifndef __MENGSEN_COUNTDOWNLATCH_H__
#define __MENGSEN_COUNTDOWNLATCH_H__

#include <condition_variable>
#include <mutex>

#include "noncopyable.h"

namespace mengsen {

/**
 * @brief count down and latch
 */
class CountDownLatch : noncopyable {
 public:
  /**
   * @param [in] number of count down
   */
  explicit CountDownLatch(int count);

  /**
   * @brief wait until count down to 0
   */
  void wait();

  /**
   * @brief count down function
   */
  void countDown();

  /**
   * @brief get count
   */
  int getCount() const;

 private:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  int count_;
};

}  // namespace mengsen

#endif  // __MENGSEN_COUNTDOWNLATCH_H__