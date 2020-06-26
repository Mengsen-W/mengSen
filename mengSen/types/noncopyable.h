/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-18 22:02:22
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-19 09:52:20
 * @Description: non-copyable Attributu class
 */

#ifndef __MENGSEN_TYPES_NONCOPYABLE_H__
#define __MENGSEN_TYPES_NONCOPYABLE_H__

namespace mengSen {

class noncopyable {
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

}  // namespace mengSen

#endif  // __MENGSEN_TYPES_NONCOPYABLE_H__

// Private inheritance to derived class, to keep empty-based-optimization