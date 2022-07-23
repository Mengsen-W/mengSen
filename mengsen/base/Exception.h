/*
 * @Date: 2020-07-22 17:32:12
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:23:04
 * @FilePath: /mengsen/mengsen/base/Exception.h
 */

#ifndef __MENGSEN_EXCEPTION_H__
#define __MENGSEN_EXCEPTION_H__

#include <exception>
#include <string>

namespace mengsen {

/**
 * @brief add stuff from exception
 */
class Exception : public std::exception {
 public:
  Exception(std::string what);
  ~Exception() noexcept override = default;

  const char* what() const noexcept override { return message_.c_str(); }
  const char* stackTrace() const noexcept { return stack_.c_str(); }

 private:
  std::string message_;
  std::string stack_;
};

}  // namespace mengsen

#endif  // __MENGSEN_EXCEPTION_H__