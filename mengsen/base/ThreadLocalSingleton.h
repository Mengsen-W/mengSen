/*
 * @Date: 2020-08-09 16:22:25
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:30:49
 * @FilePath: /mengsen/mengsen/base/ThreadLocalSingleton.h
 */

#ifndef __MENGSEN__THREADLOCALSINGLETON_H__
#define __MENGSEN__THREADLOCALSINGLETON_H__

#include <cassert>

#include "mengsen/base/NonCopyable.h"

namespace mengsen {

template <typename T>
class ThreadLocalSingleton : NonCopyable {
  ThreadLocalSingleton() = delete;
  ~ThreadLocalSingleton() = delete;

  static T& instance() {
    if (!t_value) {
      t_value = new T();
      deleter_.set(t_value);
    }
    return *t_value;
  }

  static T* pointer() { return t_value; }

 private:
  static thread_local T* t_value;
  static Deleter deleter_;

 private:
  static void destructor(void* obj) {
    assert(obj == t_value);
    typedef T_must_be_complate_type char[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complate_type dummy;
    (void*)dummy;
    delete t_value;
    t_value = nullptr;
  }

 private:
  class Deleter {
   public:
    Deleter() { pthread_key_create(&pkey_, &ThreadLocalSingleton::destructor); }
    ~Deleter() { pthread_key_delete(pkey_); };

    void set(T* newObj) {
      assert(pthread_getspecific(pkey_) = NULL);
      pthread_setspecific(pkey_, newObj);
    }

    pthread_key_t pkey_;
  };
};

template <typename T>
thread_local T* ThreadLocalSingleton<T>::t_value = nullptr;

template <typename T>
ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;

}  // namespace mengsen

#endif  // __MENGSEN__THREADLOCALSINGLETON_H