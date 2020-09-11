/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:18:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-31 20:50:27
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

/**
 * @brief switch time zone
 * @param time [uint64_t]
 * @param hour [int]
 * @return [uint64_t]
 */
uint64_t switch_timezone(uint64_t time, int hour);

/**
 * @return invalid time
 */
uint64_t invalid() { return 0; }

bool valid(uint64_t time) { return time > 0; }

/**
 * @brief add time from second parameter(seconds) to first parameter
 * @param time [uint64_t]
 * @param seconds [double]
 * @return time after add [uint64_t]
 */
uint64_t addTime(uint64_t time, double seconds) {
  uint64_t delta = static_cast<uint64_t>(seconds * kMicroSecondsPerSecond);
  return time + delta;
}

}  // namespace Timestamp

}  // namespace mengsen

#endif  // __MENGSEN_TIMESTAMP_H__