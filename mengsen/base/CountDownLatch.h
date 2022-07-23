/*
 * @Date: 2020-07-09 20:21:00
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:21:30
 * @FilePath: /mengsen/mengsen/base/CountDownLatch.h
 */

#ifndef __MENGSEN_COUNTDOWNLATCH_H__
#define __MENGSEN_COUNTDOWNLATCH_H__

#include <condition_variable>
#include <mutex>

#include "mengsen/base/NonCopyable.h"

namespace mengsen {

/**
 * @brief count down and latch
 */
class CountDownLatch : NonCopyable {
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