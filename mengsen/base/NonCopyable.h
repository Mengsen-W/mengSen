/*
 * @Date: 2020-05-18 22:02:22
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:33:45
 * @FilePath: /mengsen/mengsen/base/NonCopyable.h
 */

#ifndef __MENGSEN_TYPES_NONCOPYABLE_H__
#define __MENGSEN_TYPES_NONCOPYABLE_H__

namespace mengsen {

class NonCopyable {
 public:
  NonCopyable(const NonCopyable&) = delete;
  void operator=(const NonCopyable&) = delete;

 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;
};

}  // namespace mengsen

#endif  // __MENGSEN_TYPES_NONCOPYABLE_H__

// Private inheritance to derived class, to keep empty-based-optimization