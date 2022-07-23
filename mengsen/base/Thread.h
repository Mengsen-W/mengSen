/*
 * @Date: 2020-08-06 16:37:15
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:31:22
 * @FilePath: /mengsen/mengsen/base/Thread.h
 */

#ifndef __MENGSEN_THREAD_H__
#define __MENGSEN_THREAD_H__

#include <atomic>
#include <functional>
#include <thread>

#include "mengsen/base/CountDownLatch.h"
#include "mengsen/base/NonCopyable.h"

namespace mengsen {

class Thread : NonCopyable {
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