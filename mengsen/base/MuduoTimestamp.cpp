/*
 * @Date: 2020-07-22 21:06:17
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:34:45
 * @FilePath: /mengsen/mengsen/base/MuduoTimestamp.cpp
 */

#include "mengsen/base/MuduoTimestamp.h"

#include <inttypes.h>
#include <sys/time.h>

#include <cstdio>

using namespace muduo_timestamp;

#if DEBUG
static_assert(sizeof(Timestamp) == sizeof(int64_t),
              "Timestamp is same size as int64_t");
#endif

std::string Timestamp::toString() const {
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
  int64_t microSeconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
  snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds,
           microSeconds);
  return buf;
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const {
  char buf[64] = {0};
  time_t seconds =
      static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
  struct tm tm_time;
  gmtime_r(&seconds, &tm_time);
  if (showMicroseconds) {
    int microseconds =
        static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);

  } else {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

Timestamp Timestamp::now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}