/*
 * @Date: 2020-08-14 11:08:36
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:19:42
 * @FilePath: /mengsen/mengsen/base/AsyncLogging.h
 */

#ifndef __MENGSEN_ASYNCLOGGING_H__
#define __MENGSEN_ASYNCLOGGING_H__

#include <atomic>
#include <iostream>
#include <vector>

#include "mengsen/base/BlockingQueue.h"
#include "mengsen/base/BoundedBlockingQueue.h"
#include "mengsen/base/CountDownLatch.h"
#include "mengsen/base/LogStream.h"
#include "mengsen/base/Thread.h"

namespace mengsen {

class AsyncLogging : NonCopyable {
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
