/*
 * @Date: 2020-07-09 21:26:09
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:19:51
 * @FilePath: /mengsen/mengsen/base/BlockingQueue.h
 */

#ifndef __MENGSEN_BLOCKINGQUEUE_H__
#define __MENGSEN_BLOCKINGQUEUE_H__

#include <cassert>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace mengsen {

/**
 * @brief buffer of blocking queue
 * @template class type in queue
 */
template <typename T>
class BlockingQueue {
 public:
  BlockingQueue() : mutex_(), notEmpty_(), deque_() {}
  BlockingQueue(const BlockingQueue&) = delete;

  void put(const T& x) {
    std::lock_guard<std::mutex> lock(mutex_);
    deque_.push_back(x);
    notEmpty_.notify_one();
  }
  void put(T&& x) {
    std::lock_guard<std::mutex> lock(mutex_);
    deque_.push_back(std::move(x));
    notEmpty_.notify_one();
  }

  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (deque_.empty()) notEmpty_.wait(lock);

    assert(!deque_.empty());
    T front(std::move(deque_.front()));
    deque_.pop_front();
    return front;
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return deque_.size();
  }

 private:
  std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::deque<T> deque_;
};

}  // namespace mengsen

#endif  // __MENGSEN_BLOCKINGQUEUE_H__