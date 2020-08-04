/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-03 18:20:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-04 21:34:33
 */

#include <sys/syscall.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

class Print {
 public:
  Print() {}
  int t;
  std::vector<int> test;
};

void print(int num = 5);

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void print_pid() { std::cout << gettid() << std::endl; }

void print(int num) {
  for (int i = 0; i < num; ++i) {
    sleep(1);
    std::cout << "Thread"
              << " + " << i << std::endl;
  }
}

void test() {
  Print p;
  // must be initialization for c-style type
  p.t = 1;
  std::cout << p.t << std::endl;
  p.test.emplace_back(1);
}

void test_pid() {
  print_pid();
  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i) {
    threads.push_back(std::thread(print_pid));
  }

  for (std::thread& t : threads) {
    t.join();
  }
}

int main() {
  // test();
  // std::vector<std::thread> threads;
  // std::cout << "begin" << std::endl;
  // threads.emplace_back(std::move(std::thread(std::bind(print, 5))));
  // threads[0].join();
  // std::cout << "join finished" << std::endl;

  test_pid();
  return 0;
}
