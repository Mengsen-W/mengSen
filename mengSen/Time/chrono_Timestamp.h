/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:18:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-23 22:29:56
 */

#ifndef __MENGSEN_CHRONO_TIMESTAMP_H__
#define __MENGSEN_CHRONO_TIMESTAMP_H__

#include <chrono>
#include <string>

namespace mengsen {

class timestamp {
 public:
  enum Precision : uint8_t {
    second,
    centisecond,
    millisecond,
    microsecond,
    nanosecond
  };

  timestamp() {}
  timestamp(const timestamp&) {}

  using time_point =
      std::chrono::time_point<std::chrono::high_resolution_clock>;

  /**
   * @brief get now time with T type
   * @param void
   * @return [T]
   */
  template <typename T>
  static T now();

  template <typename T>
  static std::string toString_now();

  /**
   * @brief convert type From to type To
   * @param f [From &]
   * @return [To]
   */
  template <typename From, typename To>
  static To convert(From& f, Precision p = Precision::second);
};

}  // namespace mengsen

#endif  // __MENGSEN_CHRONO_TIMESTAMP_H__