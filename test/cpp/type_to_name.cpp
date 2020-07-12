/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-12 12:32:58
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-12 12:39:31
 */

#include <cxxabi.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

class Test {
 public:
  static void test() {}
};

template <class T>
const char* TypeToName() {
  static const char* s_name =
      abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
  return s_name;
}

void test() {
  std::cout << TypeToName<std::vector<std::vector<std::string>>>() << std::endl;
}

int main() {
  test();
  return 0;
}