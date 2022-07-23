/*
 * @Date: 2020-07-09 20:58:18
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:25:09
 * @FilePath: /mengsen/mengsen/base/WeakCallback.h
 */

#ifndef __MENGSEN_WEAKCALLBACK_H__
#define __MENGSEN_WEAKCALLBACK_H__

#include <functional>
#include <memory>

namespace mengsen {

/**
 * @brief: weakCallback object
 * @description:
 *  typename CLASS weak_ptr object
 *  typename ...ARGS pass to function with CLASS*
 */
template <typename CLASS, typename... Args>
class WeakCallback {
 public:
  WeakCallback(const std::weak_ptr<CLASS>& object,
               std::function<void(CLASS*, Args...)> function)
      : object_(object), function_(function) {}

  void operator()(Args&&... args) const {
    std::shared_ptr<CLASS> ptr(object_.lock());
    if (ptr)
      // CLASS object_ alive
      function_(ptr.get(), std::forward<Args>(args)...);
  }

 private:
  std::weak_ptr<CLASS> object_;
  std::function<void(CLASS*, Args...)> function_;
};

/**
 * @brief: make weakCallback object with shared_ptr and function ptr
 * @description:
 *  typename CLASS weak_ptr object
 *  typename ...ARGS pass to function with CLASS*
 */
template <typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(
    const std::shared_ptr<CLASS>& object, void (CLASS::*function)(ARGS...)) {
  return WeakCallback<CLASS, ARGS...>(object, function);
}

/**
 * @brief: make weakCallback object with shared_ptr and const function ptr
 * @description:
 *  typename CLASS weak_ptr object
 *  typename ...ARGS pass to function with CLASS*
 */
template <typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(
    const std::shared_ptr<CLASS>& object,
    void (CLASS::*function)(ARGS...) const) {
  return WeakCallback<CLASS, ARGS...>(object, function);
}

}  // namespace mengsen

#endif  // __MENGSEN_WEAKCALLBACK_H__