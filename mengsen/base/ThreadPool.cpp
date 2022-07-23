/*
 * @Date: 2020-08-06 21:48:46
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:30:38
 * @FilePath: /mengsen/mengsen/base/ThreadPool.cpp
 */

#include "mengsen/base/ThreadPool.h"

#include <cassert>

namespace mengsen {

ThreadPool::ThreadPool(const std::string& name)
    : mutex_(),
      notEmpty_(),
      notFull_(),
      name_(name),
      threads_(),
      queue_(),
      maxQueueSize_(0),
      running_(false) {}

ThreadPool::~ThreadPool() {
  if (running_) stop();
}

size_t ThreadPool::queueSize() {
  std::lock_guard<std::mutex> lock(mutex_);
  return queue_.size();
}

void ThreadPool::start(int numThreads) {
  assert(threads_.size() == 0);
  running_ = true;
  threads_.reserve(numThreads);

  for (int i = 0; i < numThreads; ++i) {
    char id[32];
    snprintf(id, sizeof(id), "%d", i + 1);
    threads_.emplace_back(std::make_unique<Thread>(
        std::bind(&ThreadPool::runInThread, this), name_ + id));
    threads_[i]->start();
  }

  if (numThreads == 0 && threadInitCallback_) threadInitCallback_();
}

void ThreadPool::stop() {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    running_ = false;
    notEmpty_.notify_all();
  }

  for (auto& thread : threads_) thread->join();
}

void ThreadPool::run(Task task) {
  if (threads_.empty())
    task();
  else {
    std::unique_lock<std::mutex> lock(mutex_);
    while (isFull()) notFull_.wait(lock);
    assert(!isFull());
    queue_.push_back(task);
    notEmpty_.notify_one();
  }
}

ThreadPool::Task ThreadPool::take() {
  Task task;
  std::unique_lock<std::mutex> lock(mutex_);
  while (queue_.empty() && running_) {
    notEmpty_.wait(lock);
  }

  if (!queue_.empty()) {
    task = queue_.front();
    queue_.pop_front();
    if (maxQueueSize_ > 0) {
      notFull_.notify_one();
    }
  }
  return task;
}

bool ThreadPool::isFull() const {
  return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
}

void ThreadPool::runInThread() {
  try {
    if (threadInitCallback_) threadInitCallback_();
    while (running_) {
      Task this_take(take());
      if (this_take) this_take();
    }
  } catch (const Exception& ex) {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
    abort();
  } catch (const std::exception& ex) {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    abort();
  } catch (...) {
    fprintf(stderr, "unknown exception caught in ThreadPool %s\n",
            name_.c_str());
    throw;  // rethrow
  }
}

}  // namespace mengsen