/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-09 16:12:45
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-09 16:19:42
 */

#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>

class Test {
 public:
  Test() { memset(this, 0, sizeof(Test)); }

 private:
  char mem[32];
};

Test *t = nullptr;

void fun() {
  t = new Test();
  std::cout << t << std::endl;
  // attempting double-free in thread T2
  sleep(1);
  // every thread try to delete pointer
  delete t;
}

int main() {
  std::thread t1(fun);
  std::thread t2(fun);

  t1.join();
  t2.join();
  std::cout << t << std::endl;
  return 0;
}