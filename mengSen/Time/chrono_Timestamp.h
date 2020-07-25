/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:18:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 10:26:49
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
  template <typename T = uint64_t>
  static T now();

  static std::string toString_now(Precision p = Precision::second);

  /**
   * @brief convert type From to type To
   * @param f [From &]
   * @return [To]
   */
  template <typename From = uint64_t, typename To = std::string>
  static To convert(From&& f, Precision p = Precision::second);
};

}  // namespace mengsen

#endif  // __MENGSEN_CHRONO_TIMESTAMP_H__