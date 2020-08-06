/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-22 17:32:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 18:09:02
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