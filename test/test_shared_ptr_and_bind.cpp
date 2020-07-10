/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-10 16:07:38
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-10 16:45:29
 */

#include <functional>
#include <iostream>
#include <memory>

class Test {
 public:
  void test() {}
};

void test(std::shared_ptr<Test> &t1) {
  std::cout << t1.use_count() << std::endl;
  auto b = std::bind(&Test::test, t1);
  std::cout << t1.use_count() << std::endl;
  // 最好的写法
  std::function<void()> f1 = std::bind(&Test::test, t1);

  //! no suitable user-defined
  //! conversion from "std::_Bind<std::__remove_cv_t<void (Test::*)()> ()>"
  //! to "std::function<void ()>" exists
  // std::function<void()> f2 = std::bind(&Test::test);

  std::function<void(Test *)> f2 = &Test::test;
  std::cout << t1.use_count() << std::endl;
  b();
  std::cout << t1.use_count() << std::endl;
  f1();
  f2(t1.get());
  std::cout << t1.use_count() << std::endl;
  return;
}

int main() {
  std::shared_ptr<Test> t1 = std::make_shared<Test>();
  std::cout << t1.use_count() << std::endl;
  test(t1);
  std::cout << t1.use_count() << std::endl;
  return 0;
}