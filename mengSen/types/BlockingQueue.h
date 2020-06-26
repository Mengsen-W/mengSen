/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-23 21:26:24
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-23 22:19:53
 */

#ifndef __MENGSEN_TYPES_BLOCKINGQUEUE_H__
#define __MENGSEN_TYPES_BLOCKINGQUEUE_H__

#include <cassert>
#include <condition_variable>
#include <mutex>  // for unique_mutex
#include <queue>

#include "./noncopyable.h"

namespace mengSen {

template <typename T>
class BlockingQueue : noncopyable {
 public:
  BlockingQueue() : mutex_(), notEmpty_(), queue_() {}

  void put(const T& x) {
    std::lock_guard<std::mutex>(mutex_);
    queue_.push_back(x);
    notEmpty_.notify_all();
  }

  void put(T&& x) {
    std::lock_guard<std::mutex>(mutex_);
    queue_.emplace_back(x);
    notEmpty_.notify_all();
  }

  T take() {
    std::unique_lock<std::mutex> unique_mutex(mutex_);
    notEmpty_.wait(unique_mutex, [&queue_] { return queue_.empty(); });

    assert(!queue_.empty());

    T front(std::move(queue_.front()));
    queue_.pop_front();
    return front;
  }

  size_t size() const {
    std::lock_guard<std::mutex>(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::deque<T> queue_;
};

}  // namespace mengSen

#endif  // __MENGSEN_TYPES_BLOCKINGQUEUE_H__