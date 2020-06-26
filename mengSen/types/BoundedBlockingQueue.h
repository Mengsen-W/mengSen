/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-23 21:46:40
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-23 22:30:50
 */

#ifndef __MENGSEN_TYPES_BOUNDEDBLOCKINGQUEUE_H__
#define __MENGSEN_TYPES_BOUNDEDBLOCKINGQUEUE_H__

#include <boost/circular_buffer.hpp>
#include <condition_variable>
#include <mutex>

#include "./noncopyable.h"

namespace mengSen {

template <typename T>
class BoundedBlockingQueue : noncopyable {
 public:
 //TODO
  explicit BoundedBlockingQueue() : mutex_(), notEmpty_(){}

 private:
  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::condition_variable notFull_;
  boost::circular_buffer<T> queue_;
};

}  // namespace mengSen

#endif  //__MENGSEN_TYPES_BOUNDEDBLOCKINGQUEUE_H__