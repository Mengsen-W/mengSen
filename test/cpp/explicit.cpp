/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 17:09:29
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 17:26:38
 */

#include <iostream>
#include <string>

class Test {
 public:
  // 一个将死的量放在常量区不好，应该放在堆或者栈上
  explicit Test(std::string&& name) : name_(std::move(name)) {
    std::cout << "const &&" << std::endl;
  }
  explicit Test(const std::string& name) : name_(name) {
    std::cout << "const &" << std::endl;
  }

 private:
  std::string name_;
};

void test() {
  std::string s = "wms";

  Test t1(s);
  Test t2(std::move(s));

  Test t3(std::move(t1));

  return;
}

int main() {
  test();
  return 0;
}