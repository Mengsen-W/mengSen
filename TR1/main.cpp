/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 22:24:55
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 22:31:37
 */

#include <iostream>
#include <vector>

#include "CurrentThread.h"
#include "Log.h"
#include "Thread.h"
#include "ThreadPool.h"

// pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void function() {
  LOG_DEBUG << mengsen::CurrentThread::tidString() << " "
            << mengsen::CurrentThread::name() << " "
            << "test log";
}

void test_thread() {
  std::string name = "wms";
  for (int i = 0; i < 10; i++) {
    mengsen::Thread(function, name).start();
  }

  std::cout << "finish start" << std::endl;
}

mengsen::ThreadPool tp;

void test_ThreadPool() {
  for (int i = 0; i < 10000000; i++) tp.run(function);
  tp.start(std::thread::hardware_concurrency() + 2);
}

int main() {
  mengsen::log::initialize(mengsen::NonGuaranteedLogger(105),
                           "/home/Mengsen/mengSen/logfile/", "mengsen", 10);
  LOG_DEBUG << mengsen::CurrentThread::tidString() << " "
            << mengsen::CurrentThread::name() << " "
            << "test log";
  // test_thread();
  test_ThreadPool();

  while (tp.queueSize() != 0) mengsen::CurrentThread::yield();

  return 0;
}