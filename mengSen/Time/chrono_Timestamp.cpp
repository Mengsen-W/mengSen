/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:29:54
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 10:26:50
 */

#include "./chrono_Timestamp.h"

#include <cassert>
#include <cstring>
#include <iostream>

using namespace mengsen;

template <>
uint64_t timestamp::now<uint64_t>() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

/**
 * Non-corresponding Precision and return type may be lose precision will
 * static_assert abort
 */

template <>
timestamp::time_point timestamp::now<timestamp::time_point>() {
  return std::chrono::high_resolution_clock::now();
}

template <>
std::string timestamp::convert(uint64_t&& time, timestamp::Precision p) {
  // assert(p > 4U);
  // format int64_t to tm*
  std::chrono::nanoseconds duration{time};
  std::chrono::high_resolution_clock::time_point time_point{duration};
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  tm* gmtime = std::gmtime(&time_now);

  char buffer[32];
  memset(buffer, 0, 32);
  strftime(buffer, 20, "%Y-%m-%d %T", gmtime);

  uint64_t t = time % 1000000000;
  buffer[19] = '.';
  sprintf(buffer + 20, "%09lu", t);

  switch (p) {
    case timestamp::Precision::second: {
      memset(buffer + 19, 0, 13);
    }
    case timestamp::Precision::centisecond: {
      memset(buffer + 22, 0, 10);
      break;
    }
    case timestamp::Precision::millisecond: {
      memset(buffer + 23, 0, 9);
      break;
    }
    case timestamp::Precision::microsecond: {
      memset(buffer + 26, 0, 6);
      break;
    }
    case timestamp::Precision::nanosecond: {
      memset(buffer + 29, 0, 3);
      break;
    }
  }
  return buffer;
}

std::string timestamp::toString_now(Precision p) {
  return timestamp::convert<uint64_t, std::string>(timestamp::now<uint64_t>(),
                                                   p);
}

#if 1

int main() {
  std::cout << timestamp::toString_now(timestamp::second) << std::endl;
  std::cout << timestamp::toString_now(timestamp::centisecond) << std::endl;
  std::cout << timestamp::toString_now(timestamp::millisecond) << std::endl;
  std::cout << timestamp::toString_now(timestamp::microsecond) << std::endl;
  std::cout << timestamp::toString_now(timestamp::nanosecond) << std::endl;
  return 0;
}

#endif