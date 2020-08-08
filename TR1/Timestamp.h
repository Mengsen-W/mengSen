/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:18:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-08 11:33:26
 */

#ifndef __MENGSEN_TIMESTAMP_H__
#define __MENGSEN_TIMESTAMP_H__

#include <chrono>
#include <string>

namespace mengsen {

namespace Timestamp {

static const int kMicroSecondsPerSecond = 1000 * 1000;

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
 * @param f [From &&]
 * @return [To]
 */
template <typename From = uint64_t, typename To = std::string>
To convert(From f, Precision p = Precision::second);

}  // namespace Timestamp

}  // namespace mengsen

#endif  // __MENGSEN_TIMESTAMP_H__