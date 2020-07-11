/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 21:36:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 21:52:04
 */

#include <iostream>
#include <vector>

template <typename T>
static bool T_must_be_complate_type() {
  typedef char T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1];
  T_must_be_complate_type dummy;
  (void)dummy;
  return true;
}

class Test1 {};
class Test2;

int main() {
  std::cout << sizeof(Test1) << std::endl;
  std::cout << T_must_be_complate_type<Test1>() << std::endl;
  // std::cout << sizeof(Test2) << std::endl;
  // std::cout << T_must_be_complate_type<Test2>() << std::endl;
  return 0;
}