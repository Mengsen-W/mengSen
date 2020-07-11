/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 17:19:36
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 17:34:59
 */

#include <iostream>

class Num {
 public:
  Num() : num_(0) { std::cout << "Constructor" << std::endl; }
  Num(int num) : num_(num) { std::cout << "Constructor" << std::endl; }
  Num(const Num& other) {
    num_ = other.num_;
    std::cout << "copy" << std::endl;
  }
  Num& operator=(const Num& other) {
    num_ = other.num_;
    return *this;
  }
  ~Num() { std::cout << "Destructor" << std::endl; }

  int num_;
};

int main() {
  Num num1(1);
  Num num2(2);
  Num num3(3);
  // 如果不是返回引用则 (num3 = num2) 先算后产生一个副本
  // num1 赋值给了这个副本
  (num3 = num2) = num1;
  // 如果是引用的则 (num3 = num2) 返回 num3 本身
  // 这样 num1 就可以直接赋值给 num3 了
  std::cout << "num1 = " << num1.num_ << std::endl;
  std::cout << "num2 = " << num2.num_ << std::endl;
  std::cout << "num3 = " << num3.num_ << std::endl;
  return 0;
}

/* return reference
Constructor
Constructor
Constructor
num1 = 1
num2 = 2
num3 = 1
Destructor
Destructor
Destructorr
*/

/* return object
Constructor
Constructor
Constructor
copy
copy
Destructor
Destructor
num1 = 1
num2 = 2
num3 = 2
Destructor
Destructor
Destructor
*/