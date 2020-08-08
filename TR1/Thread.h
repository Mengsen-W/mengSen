/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 16:37:15
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 22:44:34
 */

#ifndef __MENGSEN_THREAD_H__
#define __MENGSEN_THREAD_H__

#include <atomic>
#include <functional>
#include <thread>

#include "CountDownLatch.h"
#include "noncopyable.h"

namespace mengsen {

class Thread : noncopyable {
 public:
  using ThreadFunc = std::function<void()>;
  explicit Thread(ThreadFunc, const std::string& name = std::string());
  explicit Thread(ThreadFunc, std::string&& name);
  ~Thread();

  Thread(const Thread&) = delete;
  Thread operator=(const Thread&) = delete;

  bool start();
  bool join();

  bool started() const { return started_; }
  pid_t tid() const { return tid_; }
  const std::string& name() const { return name_; }

  static int32_t numCreated() { return numCreated_; }

 private:
  void setDefaultName();

  bool started_;
  bool joined_;
  std::thread threadObj;
  pid_t tid_;
  ThreadFunc func_;
  std::string name_;
  CountDownLatch latch_;
  static std::atomic<int32_t> numCreated_;
};

}  // namespace mengsen

#endif  // __MENGSEN_THREAD_H__