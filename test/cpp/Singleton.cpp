/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 16:39:42
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 17:56:09
 */

// effective c++ item4 P31
class Singleton_Lazy {
 private:
  Singleton_Lazy(){};
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

 private:
  Singleton_Eager() {}
  ~Singleton_Eager() {}
  Singleton_Eager(const Singleton_Eager&);
  Singleton_Eager& operator=(const Singleton_Eager&);

 public:
  static Singleton_Eager& getInstance() { return instance; }
};

// initialize default
Singleton_Eager Singleton_Eager::instance;

int main() { return 0; }