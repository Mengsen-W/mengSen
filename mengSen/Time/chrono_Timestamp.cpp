/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:29:54
 * @Last Modified by:   Mengsen.Wang
 * @Last Modified time: 2020-07-23 22:29:54
 */

#include "./chrono_Timestamp.h"

using namespace mengsen;

template <>
int64_t timestamp::now() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

template <>
timestamp::time_point timestamp::now() {
  return std::chrono::high_resolution_clock::now();
}

template <>
std::string timestamp::convert(int64_t& time, timestamp::Precision p) {
  // format int64_t to tm*
  std::chrono::microseconds duration{time};
  std::chrono::high_resolution_clock::time_point time_point{duration};
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  tm* gmtime = std::gmtime(&time_now);

  char buffer[32];
  strftime(buffer, 32, "%Y-%m-%d %T", gmtime);

  switch (p) {
    case timestamp::Precision::second: {
    }
    case timestamp::Precision::centisecond: {
    }
    case timestamp::Precision::millisecond: {
    }
    case timestamp::Precision::microsecond: {
    }
    case timestamp::Precision::nanosecond: {
    }
  }
}