/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-18 22:11:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 18:04:37
 * @Description: copyable Attribute class
 */

#ifndef __MENGSEN_COPYABLE_H__
#define __MENGSEN_COPYABLE_H__

namespace mengsen {

// any derived class of copyable should be a value type

class copyable {
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace mengsen

#endif  // __MENGSEN_COPYABLE_H__