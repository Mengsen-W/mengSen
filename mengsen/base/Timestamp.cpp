/*
 * @Date: 2020-07-23 22:29:54
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 12:40:55
 * @FilePath: /mengsen/mengsen/base/Timestamp.cpp
 */

#include "mengsen/base/Timestamp.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace mengsen {

namespace Timestamp {

template <>
Timestamp::Time_uint64_t now<Timestamp::Time_uint64_t>() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

template <>
time_t now<time_t>() {
  Timestamp::Time_uint64_t time =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  std::chrono::nanoseconds duration{time};
  std::chrono::high_resolution_clock::time_point time_point{duration};
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  return time_now;
}

template <>
std::string convert(Timestamp::Time_uint64_t time, Precision p) {
#if DEBUG
  assert(p < Precision::nanosecond);
#endif
  // format int64_t to tm*
  static char buffer[32];
  std::chrono::nanoseconds duration{time};
  std::chrono::high_resolution_clock::time_point time_point{duration};
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  tm* gmtime = std::gmtime(&time_now);

  strftime(buffer, 18, "%Y%m%d %T", gmtime);

  Timestamp::Time_uint64_t t = time % 1000000000;
  buffer[17] = '.';
  sprintf(buffer + 18, "%09lu", t);

  switch (p) {
    case Precision::second: {
      memset(buffer + 17, 0, 15);
      break;
    }
    case Precision::centisecond: {
      memset(buffer + 20, 0, 12);
      break;
    }
    case Precision::millisecond: {
      memset(buffer + 21, 0, 11);
      break;
    }
    case Precision::microsecond: {
      memset(buffer + 24, 0, 8);
      break;
    }
    case Precision::nanosecond: {
      memset(buffer + 27, 0, 5);
      break;
    }
  }
  return buffer;
}

std::string toString_now(Precision p) {
  return convert<Timestamp::Time_uint64_t, std::string>(now<Timestamp::Time_uint64_t>(), p);
}

Timestamp::Time_uint64_t switch_timezone(Timestamp::Time_uint64_t time, int hour) {
  static Timestamp::Time_uint64_t zone = 3600000000000UL;
  time += hour * zone;
  return time;
}

}  // namespace Timestamp

};  // namespace mengsen