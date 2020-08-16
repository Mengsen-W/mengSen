/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 11:08:36
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 17:41:05
 */

#ifndef __MENGSEN_ASYNCLOGGING_H__
#define __MENGSEN_ASYNCLOGGING_H__

#include <atomic>
#include <iostream>
#include <vector>

#include "BlockingQueue.h"
#include "BoundedBlockingQueue.h"
#include "CountDownLatch.h"
#include "LogStream.h"
#include "Thread.h"

namespace mengsen {

class AsyncLogging : noncopyable {
 public:
  AsyncLogging(const std::string& basename, off_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging() {
    if (running_) {
      stop();
    }
  }

  void append(const char* logline, int len);

  void start() {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop() {
    running_ = false;
    cond_.notify_one();
    thread_.join();
  }

 private:
  void threadFunc();

  typedef mengsen::detail::FixedBuffer<mengsen::detail::kLargeBuffer> Buffer;
  typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
  typedef BufferVector::value_type BufferPtr;

  const int flushInterval_;
  std::atomic<bool> running_;
  const std::string basename_;
  const off_t rollSize_;
  mengsen::Thread thread_;
  mengsen::CountDownLatch latch_;
  std::mutex mutex_;
  std::condition_variable cond_;
  BufferPtr currentBuffer_;
  BufferPtr nextBuffer_;
  BufferVector buffers_;
};

}  // namespace mengsen

#endif  // __MENGSEN_ASYNCLOGGING_H__
