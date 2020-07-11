/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-12 16:40:43
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-12 17:20:02
 */

#include <iostream>

template <typename T>
void test(const T t) {
  std::cout << t << std::endl;
  return;
}

int main() {
  char* c = "wms";
  const char* cc = "wmscc";

  // std::cout << *cp << std::endl;
  while (*c != '\0') {
    std::cout << *c;
    ++c;
  }
  std::cout << cc << std::endl;

  const int t = 1;
  int t2 = 2;
  test(t2);
  return 0;
}