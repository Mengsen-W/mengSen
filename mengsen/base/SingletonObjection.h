/*
 * @Date: 2020-07-11 16:23:22
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:33:05
 * @FilePath: /mengsen/mengsen/base/SingletonObjection.h
 */

#ifndef __MENGSEN_SINGLETON_H__
#define __MENGSEN_SINGLETON_H__

#include <pthread.h>

#include <cstdlib>

namespace {

template <typename T>
struct has_no_destroy {
  template <typename C>
  static char test(decltype(&C::no_destroy));
  template <typename C>
  static int test(...);
  const static bool value = sizeof(test<T>(0)) == 1;
};

}  // namespace

namespace mengsen {

template <typename T>
class Singleton {
 public:
  Singleton() = delete;
  ~Singleton() = delete;

  static T& instance() {
    pthread_once(&ponce_, &Singleton::init);
    assert(value_ != nullptr);
    return *value_;
  }

 private:
  static void init() {
    value_ = new T();
    if (!has_no_destroy<T>::value) atexit(destory);
  }

  static void destory() {
    typedef char T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complate_type dummy;
    (void)dummy;

    delete value_;
    value_ = nullptr;
  }

 private:
  static pthread_once_t ponce_;
  static T* value_;
};

template <typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;
template <typename T>
T* Singleton<T>::value_ = nullptr;

}  // namespace mengsen

#endif  // __MENGSEN_SINGLETON_H__