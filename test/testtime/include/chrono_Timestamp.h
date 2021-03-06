/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:18:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-04 22:24:25
 */

#ifndef __MENGSEN_CHRONO_TIMESTAMP_H__
#define __MENGSEN_CHRONO_TIMESTAMP_H__

#include <chrono>
#include <string>

namespace mengsen_timestamp {

enum Precision : uint8_t {
  second,
  centisecond,
  millisecond,
  microsecond,
  nanosecond
};

using mengsen_time_point =
    std::chrono::time_point<std::chrono::high_resolution_clock>;

/**
 * @brief get now time with T type
 * @param void
 * @return [T]
 */
template <typename T = uint64_t>
T now();

std::string toString_now(Precision p = Precision::second);

/**
 * @brief convert type From to type To
 * @param f [From &]
 * @return [To]
 */
template <typename From = uint64_t, typename To = std::string>
To convert(From&& f, Precision p = Precision::second);

}  // namespace mengsen_timestamp

#endif  // __MENGSEN_CHRONO_TIMESTAMP_H__