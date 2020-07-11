/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 21:26:38
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 21:37:27
 */

#include <iostream>

template <typename T>
struct has_no_destroy {
  template <typename C>
  static char test(decltype(&C::no_destroy));
  template <typename C>
  static int test(...);
  const static bool value = sizeof(test<T>(0)) == 1;
};

class Test1 {
 public:
  void no_destroy() {}
};

class Test2 {
 public:
  int no_destory = 0;
};

class Test3 {
 private:
  int no_destroy = 0;
};

class Test4 {
  static int no_destroy;
};

class Test5 {
 public:
  static int no_destroy;
};

int Test4::no_destroy = 0;
int Test5::no_destroy = 0;

void test() {
  std::cout << has_no_destroy<Test1>::value << std::endl;
  std::cout << has_no_destroy<Test2>::value << std::endl;
  std::cout << has_no_destroy<Test3>::value << std::endl;
  std::cout << has_no_destroy<Test4>::value << std::endl;
  std::cout << has_no_destroy<Test5>::value << std::endl;
}

int main() {
  test();
  return 0;
}