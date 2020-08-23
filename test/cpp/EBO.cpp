/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-23 21:03:17
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-23 21:12:50
 */

#include <iostream>

class noncopyable {
 public:
 protected:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;
  //  private:
  // 因为不能直接实例一个 noncopyable 实例
  // 所以构造一定在protected
  noncopyable() = default;
  ~noncopyable() = default;
  // 也不能声明 private 这样继承子类就不能访问到积累的构造函数
  // 也就不能构造基类
};

class copyable {
 protected:
  copyable() = default;
  ~copyable() = default;
};

class T : private noncopyable {
 public:
  T() : t(0) {}
  int get() const { return t; }

 private:
  int t;
};

int main() {
  T t1;
  T t2 = t1;
  std::cout << t2.get() << std::endl;
  std::cout << sizeof(t1) << std::endl;
  return 0;
}