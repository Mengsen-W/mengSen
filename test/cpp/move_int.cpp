/*
 * @Author: Mengsen.Wang
 * @Date: 2020-09-14 22:22:48
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-14 22:46:15
 */

#include <iostream>  // this include <bits/move.h>

class Test {
 public:
  Test(int a) : fd(a) {}
  Test(Test&& t) : fd(std::move(t.fd)) {}

 private:
  const int fd;
};

int main() {
  int a = 1;
  std::cout << a << std::endl;

  int b = std::move(a);
  std::cout << b << std::endl;

  const int c = 2;
  int d = std::move(c);
  std::cout << d << std::endl;

  Test t1(1);
  Test t2(std::move(t1));

  return 0;
}