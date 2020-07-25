/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-25 10:23:40
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 11:04:14
 */

#include "./include/simple_time.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>

uint64_t timestamp_now() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}

void format_timestamp(std::ostream& os, uint64_t timestamp) {
  std::chrono::nanoseconds duration{timestamp};
  std::chrono::high_resolution_clock::time_point time_point{duration};
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  tm* gmtime = std::gmtime(&time_now);

  char buffer[32];
  // converts the time format to the format we want
  strftime(buffer, 32, "%Y%m%d %T.", gmtime);
  char microseconds[7];
  sprintf(microseconds, "%06lu", timestamp % 1000000);
  os << buffer << microseconds << std::endl;
}