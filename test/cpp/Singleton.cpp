/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 16:39:42
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 17:56:09
 */

#include <iostream>

// effective c++ item4 P31
class Singleton_Lazy {
 private:
  Singleton_Lazy() { std::cout << "init singleton lazy" << std::endl; };
  ~Singleton_Lazy(){};
  Singleton_Lazy(const Singleton_Lazy&);
  Singleton_Lazy& operator=(const Singleton_Lazy&);

 public:
  static Singleton_Lazy& getInstance() {
    // local static 没有用堆资源不需要释放
    static Singleton_Lazy instance;
    // return reference 保证单例，每个线程得到一致
    return instance;
  }
};

class Singleton_Eager {
 private:
  static Singleton_Eager instance;
  int test;

 private:
  Singleton_Eager() { std::cout << "init singleton eager" << std::endl; }
  ~Singleton_Eager() {}
  Singleton_Eager(const Singleton_Eager&);
  Singleton_Eager& operator=(const Singleton_Eager&);

 public:
  static Singleton_Eager& getInstance() { return instance; }
};

// initialize default
// 不写这个会有链接错误，也就是说对于类内的部分编译期是不会初始化的
Singleton_Eager Singleton_Eager::instance;

static int a = 10;
static int b;

int main() {
  static int c = 10;
  static int d = 10;
  std::cout << &a << std::endl;
  std::cout << &b << std::endl;
  std::cout << &c << std::endl;
  std::cout << &d << std::endl;
  std::cout << &(Singleton_Lazy::getInstance()) << std::endl;
  std::cout << &(Singleton_Eager::getInstance()) << std::endl;
  std::cout << sizeof(int) << std::endl;
  std::cout << sizeof(Singleton_Lazy) << std::endl;
  std::cout << sizeof(Singleton_Eager) << std::endl;
  return 0;
}

/*
init singleton eager
0x4b5010
0x4e4038
0x4b5014
0x4b5018
init singleton lazy
0x4b7e50
0x4e4030
4
1
4
*/

/*
首先初始化和分配内存是分开的
饿汉式分配在未初始化静态区 bss
懒汉式分配在初始化静态区 data
饿汉在程序开始的时候初始化
懒汉在第一次调用的时候初始化

懒汉模式 声明和定义是在函数内的 static 会默认0初始化 存放在 data 区
调用的时候会构造因为构造函数不发生在编译期 饿汉模式 声明类成员变量 static
不初始化 放在 bss
区，如果不在类外定义会发生链接错误，是因为链接的过程中会检查符号表判断是否有效
*/