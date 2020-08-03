/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-03 18:20:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-03 21:37:18
 */

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

void print(int num) {
  for (int i = 0; i < num; ++i) {
    sleep(1);
    std::cout << "Thread"
              << " + " << i << std::endl;
  }
}

void test() {
  Print p;
  std::cout << p.t << std::endl;
  p.test.emplace_back(1);
}

int main() {
  test();
  // std::vector<std::thread> threads;
  // std::cout << "begin" << std::endl;
  // threads.emplace_back(std::move(std::thread(std::bind(print, 5))));
  // threads[0].join();
  // std::cout << "join finished" << std::endl;
  return 0;
}
