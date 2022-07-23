/*
 * @Date: 2022-03-08 22:17:45
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-09 07:55:54
 * @FilePath: /mengsen/test.cpp
 * @Description: file content
 */

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class A {
 public:
  void call_back(function<void()> cb) { cb(); }
};

class B : public enable_shared_from_this<B> {
 public:
  void ff_a(string&& input) {
    a.call_back([this, &input]() { this->f_a(move(input)); });
    a.call_back([this, input]() mutable { this->f_a(move(input)); });
    a.call_back([a{shared_from_this()}, &input]() { a->f_a(move(input)); });
    a.call_back(
        [a{shared_from_this()}, input]() mutable { a->f_a(move(input)); });
  }
  int f_a(string&& input) {
    cout << input << endl;
    cout << shared_from_this().use_count() - 1 << endl;
    return 0;
  }
  A a;
};

int main() {
  shared_ptr<B> b_p{make_shared<B>()};
  b_p->ff_a("Hello");
  return 0;
}

// void call_back(function<void()> cb) { cb(); }

// class B : public enable_shared_from_this<B> {
//  public:
//   B() { b_ = shared_from_this(); }
//   void ff_a(string input) {
//     void (B::*fp)(string) = &B::f_a;
//     cout << shared_from_this().use_count() << endl;  // 2
//     call_back(bind(fp, shared_from_this(), input));  // 2
//   }
//   void f_a(string input) {
//     cout << input << endl;
//     cout << shared_from_this().use_count() << endl;  // 3
//     // 2
//   }
//   shared_ptr<B> f() { return shared_from_this(); }
//   shared_ptr<B> b_;
// };

// int main() {
//   shared_ptr<B> b{make_shared<B>()};  // 1
//   cout << b.use_count() << endl;
//   b->ff_a("Hello");  // 1
//   cout << b.use_count() << endl;
// }

// class X;
// class Y : public enable_shared_from_this<Y> {
//  public:
//   shared_ptr<X> x_;
// };

// class X : public enable_shared_from_this<X> {
//  public:
//   shared_ptr<Y> y_;
// };
// int main() {
//   // valgrind --leak-check=yes -s ./test.out
//   shared_ptr<Y> y{make_shared<Y>()};
//   shared_ptr<X> x{make_shared<X>()};
//   // y->x_ = x;
//   // x->y_ = y;

//   cout << x.use_count() << endl;
//   cout << y.use_count() << endl;
//   return 0;
// }
