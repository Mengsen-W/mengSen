/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-13 21:01:16
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-13 21:24:03
 */

#ifndef __MENGSEN_THREAD_H__
#define __MENGSEN_THREAD_H__

#include <pthread.h>

#include <atomic>
#include <functional>
#include <string>

#include "../base/CountDownLatch.h"

namespace mengsen {

class Thread {
 public:
  typedef std::function<void()> ThreadFunc;
  explicit Thread(ThreadFunc, const std::string& name = std::string());
  explicit Thread(ThreadFunc, std::string&& name = std::string());
  ~Thread();

  // non-copyable
  Thread(const Thread& t) = delete;
  Thread operator=(const Thread& t) = delete;

  void start();
  int join();

  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

  static int32_t numCreated() { return numCreated_; }

 private:
  void setDefaultName();

 private:
  // whether or not started
  bool started_;
  // whether or not joined
  bool joined_;
  pthread_t pthreadId_;
  pid_t tid_;
  // thread function
  ThreadFunc func_;
  // thread name
  std::string name_;
  CountDownLatch latch_;
  // number of created threads
  static std::atomic_int32_t numCreated_;
};

}  // namespace mengsen

#endif  // __MENGSEN_THREAD_H__