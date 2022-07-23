/*
 * @Date: 2020-07-09 20:37:49
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:21:18
 * @FilePath: /mengsen/mengsen/base/CountDownLatch.cpp
 */

#include "mengsen/base/CountDownLatch.h"

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
