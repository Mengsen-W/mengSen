/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-06 21:29:09
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-06 22:09:57
 */

#ifndef __MENGSEN_THREAD_H__
#define __MENGSEN_THREAD_H__

#include <functional>

namespace mengsen_thread {

class Thread {
 public:
  typedef std::function<void()> ThreadFunc;

 private:
  void setDefaultName();

  bool started_;
  bool joined_;
};

}  // namespace mengsen_thread

#endif  // __MENGSEN_THREAD_H__