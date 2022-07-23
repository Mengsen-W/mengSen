/*
 * @Date: 2020-05-18 22:11:49
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:21:01
 * @FilePath: /mengsen/mengsen/base/Copyable.h
 */

#ifndef __MENGSEN_COPYABLE_H__
#define __MENGSEN_COPYABLE_H__

namespace mengsen {

/**
 * @brief any derived class of copyable should be a value type
 */
class Copyable {
 protected:
  Copyable() = default;
  ~Copyable() = default;
};

}  // namespace mengsen

#endif  // __MENGSEN_COPYABLE_H__