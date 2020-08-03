/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-06 22:08:45
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-03 21:45:27
 */

#include "ThreadPool.h"

#include <chrono>
#include <iostream>

namespace mengsen_thread {

void ThreadPool::Run() {
  while (!bailout_) {
    NextJob()();
    --jobs_left_;
    wait_var_.notify_one();
  }
}

ThreadPool::Task ThreadPool::NextJob() {
  Task task;
  std::unique_lock<std::mutex> job_lock(queue_mutex_);

  // Wait for a job if we don't have any.
  job_available_var_.wait(
      job_lock, [this]() -> bool { return task_queue_.size() || bailout_; });

  // Get job from the queue
  if (!bailout_) {
    task = task_queue_.front();
    task_queue_.pop();
  } else {
    // If we're bailing out, 'inject' a job into the queue to keep jobs_left
    // accurate.
    task = []() {};
    ++jobs_left_;
  }
  return task;
}

ThreadPool::ThreadPool(const size_t& num_workers)
    : jobs_left_(0), bailout_(false), finished_(false) {
  std::unique_lock<std::mutex> worker_lock(worker_mutex_);
  for (size_t i = 0; i < num_workers; ++i) {
    workers_.emplace_back(std::thread([this] { return this->Run(); }));
  }
}

ThreadPool::~ThreadPool() { JoinAll(); }

void ThreadPool::AddWorkers(const size_t& num_new_workers) {
  std::unique_lock<std::mutex> worker_lock(worker_mutex_);
  // int id = num_workers_;
  num_workers_ += num_new_workers;
  for (size_t i = 0; i < num_new_workers; i++) {
    workers_.emplace_back(std::thread([this] { return this->Run(); }));
  }
}

size_t ThreadPool::num_workers() const { return num_workers_; }

size_t ThreadPool::JobsRemaining() {
  std::lock_guard<std::mutex> guard(queue_mutex_);
  return task_queue_.size();
}

void ThreadPool::AddTask(const Task& job) {
  std::lock_guard<std::mutex> guard(queue_mutex_);
  task_queue_.push(job);
  ++jobs_left_;
  job_available_var_.notify_one();
}

void ThreadPool::JoinAll(const bool& wait_for_all) {
  if (!finished_) {
    if (wait_for_all) {
      WaitAll();
    }

    // note that we're done, and wake up any thread that's
    // waiting for a new job
    bailout_ = true;
    job_available_var_.notify_all();

    for (auto& w : workers_)
      if (w.joinable()) w.join();
    finished_ = true;
  }
}

void ThreadPool::WaitAll() {
  if (jobs_left_ > 0) {
    std::unique_lock<std::mutex> lk(wait_mutex_);
    wait_var_.wait(lk, [this] { return this->jobs_left_ == 0; });
    lk.unlock();
  }
}

}  // namespace mengsen_thread

#if true

void fun1() { std::cout << "func1" << std::endl; }
void fun2() { std::cout << "func2" << std::endl; }

int main() {
  mengsen_thread::ThreadPool threadPool(2);
  threadPool.AddTask(std::bind(fun1));
  threadPool.AddTask(std::bind(fun2));
  return 0;
}

#endif