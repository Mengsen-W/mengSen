/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-18 22:11:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-03 22:37:27
 * @Description: copyable Attribute class
 */

#ifndef __MENGSEN_COPYABLE_H__
#define __MENGSEN_COPYABLE_H__

namespace mengsen {

/**
 * @brief any derived class of copyable should be a value type
 */
class copyable {
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace mengsen

#endif  // __MENGSEN_COPYABLE_H__