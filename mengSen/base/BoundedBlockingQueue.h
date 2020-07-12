/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-09 21:52:02
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-09 21:57:47
 */

#ifndef __MENGSEN_BOUNDEDBLOCKINGQUEUE_H__
#define __MENGSEN_BOUNDEDBLOCKINGQUEUE_H__

#include <boost/circular_buffer.hpp>
#include <cassert>
#include <condition_variable>
#include <mutex>

namespace mengsen {

/**
 * brief: circular buffer of blocking queue
 * typename T: class type in queue
 */
template <typename T>
class BoundedBlockingQueue {
 public:
  /**
   * param: [in] int maxSize (used in circular buffer)
   */
  explicit BoundedBlockingQueue(int maxSize)
      : mutex_(), notEmpty_(), notFull_(), queue_(maxSize) {}

  void put(const T& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full()) {
      notFull_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(x);
    notEmpty_.notify_one();
  }
  void put(T&& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full()) {
      notFull_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(std::move(x));
    notEmpty_.notify_one();
  }
  T take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      notEmpty_.wait(lock);
    }
    assert(!queue_.empty());
    T front = std::move(queue_.front());
    queue_.pop_front();
    notFull_.notify_one();
    return front;
  }

  bool empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }
  bool full() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.full();
  }

  size_t size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.capacity();
  }

 private:
  std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::condition_variable notFull_;
  boost::circular_buffer<T> queue_;
};

}  // namespace mengsen

#endif  // __MENGSEN_BOUNDEDBLOCKINGQUEUE_H__