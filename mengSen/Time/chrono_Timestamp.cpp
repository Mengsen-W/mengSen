/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-23 22:29:54
 * @Last Modified by:   Mengsen.Wang
 * @Last Modified time: 2020-07-23 22:29:54
 */

#include "./chrono_Timestamp.h"

#include <cstring>
#include <iostream>

using namespace mengsen;

template <>
uint64_t timestamp::now<uint64_t>(Precision p) {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

/**
 * Non-corresponding Precision and return type may be lose precision will
 * static_assert abort
 */

// ! overload with no param
template <>
timestamp::time_point timestamp::now(Precision p) {
  return std::chrono::high_resolution_clock::now();
}

template <>
std::string timestamp::convert(uint64_t&& time, timestamp::Precision p) {
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
    default:
      memset(buffer + 19, 0, 13);
      break;
  }
  return buffer;
}

#if 1

int main() {
  uint64_t start = timestamp::now<uint64_t>();
  std::cout << start << std::endl;
  std::cout << timestamp::convert<uint64_t, std::string>(std::move(start),
                                                         timestamp::second)
            << std::endl;
  std::cout << timestamp::convert<uint64_t, std::string>(std::move(start),
                                                         timestamp::centisecond)
            << std::endl;
  std::cout << timestamp::convert<uint64_t, std::string>(std::move(start),
                                                         timestamp::millisecond)
            << std::endl;
  std::cout << timestamp::convert<uint64_t, std::string>(std::move(start),
                                                         timestamp::microsecond)
            << std::endl;
  std::cout << timestamp::convert<uint64_t, std::string>(std::move(start),
                                                         timestamp::nanosecond)
            << std::endl;
  return 0;
}

#endif