/*
 * @Date: 2020-08-09 15:44:22
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:31:09
 * @FilePath: /mengsen/mengsen/base/ThreadLocal.h
 */

#ifndef __MENGSEN_THREADLOCAL_H__
#define __MENGSEN_THREADLOCAL_H__

#include <pthread.h>

#include "mengsen/base/NonCopyable.h"

namespace mengsen {

template <typename T>
class ThreadLocal : NonCopyable {
 public:
  ThreadLocal() { pthread_key_create(&pkey_, &ThreadLocal::destructor); }
  ~ThreadLocal() { pthread_key_delete(pkey_); }

  T& value() {
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
    if (!perThreadValue) {
      T* newObj = new T();
      // 在此之前可能线程还没有自己的单独量
      // 所以每个线程自己单独 new 一块空间而不是使用 perThreadValue new
      // 这样就会导致最终 perThreadvalue 的值变成未设置之前的最后一个
      pthread_setspecific(pkey_, newObj);
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:
  static void destructor(void* x) {
    T* obj = static_cast<T*>(x);
    typedef char T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complate_type dummy;
    (void*)dummy;
    delete obj;
  }

 private:
  pthread_key_t pkey_;
};

}  // namespace mengsen

#endif  // __MENGSEN_THREADLOCAL_H__