/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-12 17:46:05
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-12 18:14:28
 */

#include <iostream>

class Test {
 public:
  Test() { std::cout << "()" << std::endl; }
  Test(Test&& t) { std::cout << "&&" << std::endl; }
};

void test(Test&& t) {
  std::cout << "enter function" << std::endl;
  Test test(std::move(t));
  std::cout << "exit function" << std::endl;
}

int main() {
  Test t;
  // Test tt(std::move(t));
  test(std::move(t));

  char* buffer = static_cast<char*>(std::malloc(10 * 1));
  buffer[1] = 'a';
  buffer[9] = 'w';
  // buffer[100] = 'w'; //memory error

  std::cout << buffer[1] << std::endl;
  std::cout << buffer[9] << std::endl;

  free(buffer);
  return 0;
}