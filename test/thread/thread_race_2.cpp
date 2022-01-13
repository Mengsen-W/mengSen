/*
 * @Date: 2022-01-11 08:44:34
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-01-13 07:37:14
 */

#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

int N = 0;

void fun() {
  for (size_t i = 0; i < 100; ++i) {
    ++N;
    usleep(1);
  }
  std::cout << "N = " << N << std::endl;
}

int test() {
  N = 0;
  std::vector<std::thread> thread_pool;
  for (size_t i = 0; i < 10; ++i) {
    thread_pool.emplace_back(std::thread(fun));
  }

  for (size_t i = 0; i < 10; ++i) {
    thread_pool[i].join();
  }

  std::cout << "finished N = " << N << std::endl;
  return 0;
}

int main() {
  for (size_t i = 0; i < 4; ++i) {
    test();
  }
}