/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 21:15:31
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 21:37:21
 */

#include <iostream>
#include <memory>
#include <string>

void test(std::unique_ptr<std::string>&& s) {
  s->push_back('a');
  std::cout << s->c_str() << std::endl;
}

void test_str(std::string s) { std::cout << s.c_str() << std::endl; }

int main() {
  std::unique_ptr s = std::make_unique<std::string>("wms");
  test(std::move(s));
  std::string ss = "asd";
  test_str(std::move(ss));
  return 0;
}