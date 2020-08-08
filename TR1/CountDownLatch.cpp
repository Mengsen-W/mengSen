/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-09 20:37:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 17:33:42
 */

#include "CountDownLatch.h"

using namespace mengsen;

CountDownLatch::CountDownLatch(int count)
    : mutex_(), condition_(), count_(count) {}

void CountDownLatch::wait() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (count_ > 0) {
    condition_.wait(lock);
  }
}

void CountDownLatch::countDown() {
  std::lock_guard<std::mutex> lock(mutex_);
  --count_;
  if (count_ == 0) {
    condition_.notify_all();
  }
}

int CountDownLatch::getCount() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return count_;
}
