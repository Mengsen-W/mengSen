/*
 * @Date: 2020-07-23 22:18:06
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 12:47:18
 * @FilePath: /mengsen/mengsen/base/Timestamp.h
 */

#ifndef __MENGSEN_TIMESTAMP_H__
#define __MENGSEN_TIMESTAMP_H__

#include <chrono>
#include <string>

namespace mengsen {

namespace Timestamp {

using Time_uint64_t = uint64_t;

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
template <typename T = Time_uint64_t>
T now();

std::string toString_now(Precision p = Precision::second);

/**
 * @brief convert type From to type To
 * @param f [From &&]
 * @return [To]
 */
template <typename From = Time_uint64_t, typename To = std::string>
To convert(From f, Precision p = Precision::second);

/**
 * @brief switch time zone
 * @param time [Time_uint64_t]
 * @param hour [int]
 * @return [Time_uint64_t]
 */
Time_uint64_t switch_timezone(Time_uint64_t time, int hour);

/**
 * @return invalid time
 */
Time_uint64_t invalid() { return 0; }

bool valid(Time_uint64_t time) { return time > 0; }

/**
 * @brief add time from second parameter(seconds) to first parameter
 * @param time [Time_uint64_t]
 * @param seconds [double]
 * @return time after add [Time_uint64_t]
 */
Time_uint64_t addTime(Time_uint64_t time, double seconds) {
  Time_uint64_t delta = static_cast<Time_uint64_t>(seconds * kMicroSecondsPerSecond);
  return time + delta;
}

}  // namespace Timestamp

}  // namespace mengsen

#endif  // __MENGSEN_TIMESTAMP_H__