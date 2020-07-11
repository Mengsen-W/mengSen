/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-23 21:00:14
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-23 21:04:05
 */

#include <iostream>

void test() {
  // static 变量先于main定义初始化
  static int a = 0;
  ++a;
  std::cout << a << std::endl;
}

int main() {
  for (int i = 0; i < 5; ++i) {
    test();
  }
  return 0;
}