/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-18 22:11:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-23 21:26:50
 * @Description: copyable Attribute class
 */

#ifndef __MENGSEN_TYPES_COPYABLE_H__
#define __MENGSEN_TYPES_COPYABLE_H__

namespace mengSen {

// any derived class of copyable should be a value type

class copyable {
 protected:
  copyable() = default;
  ~copyable() = default;
};

}  // namespace mengSen

#endif  // __MENGSEN_TYPES_COPYABLE_H__