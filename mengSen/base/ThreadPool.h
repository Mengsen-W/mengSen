/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-04 16:36:02
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-04 17:55:02
 */

#ifndef __MENGSEN_THREADPOOL_H__
#define __MENGSEN_THREADPOOL_H__

#include <cassert>
#include <deque>
#include <vector>

#include "Exception.h"
#include "Thread.h"
#include "noncopyable.h"

namespace mengsen {

class ThreadPool : noncopyable {
 public:
  typedef std::function<void()> Task;

  explicit ThreadPool(const std::string& name = std::string("threadPool"));
  ~ThreadPool();

  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb) { threadInitCallback_ = cb; }

  void start(int numThreads);
  void stop();

  const std::string& name() const { return name_; }
  size_t queueSize() const;

  void run(Task task);

 private:
  bool isFull() const;
  void runInThread();
  Task take();

  std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::condition_variable notFull_;
  std::string name_;

  Task threadInitCallback_;
  std::vector<std::unique_ptr<Thread>> threads_;
  std::deque<Task> queue_;
  size_t maxQueueSize_;
  bool running_;
};

}  // namespace mengsen

#endif  // __MENGSEN_THREADPOOL_H__