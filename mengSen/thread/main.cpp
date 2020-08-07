/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 22:24:55
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 22:31:37
 */

#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "CurrentThread.h"
#include "Thread.h"
#include "ThreadPool.h"

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void function() {
  // std::cout << mengsen::CurrentThread::tid() << std::endl;
  std::cout << mengsen::CurrentThread::name() << std::endl;
  // std::cout << mengsen::CurrentThread::isMainThread() << std::endl;
}

void test_thread() {
  std::string name = "wms";
  for (int i = 0; i < 10; i++) {
    mengsen::Thread(function, name).start();
  }

  std::cout << "finish start" << std::endl;
}

void test_ThreadPool() {
  mengsen::ThreadPool tp;
  tp.start(4);
  for (int i = 0; i < 10; i++) tp.run(function);

  sleep(2);
}

int main() {
  // test_thread();
  test_ThreadPool();
  return 0;
}