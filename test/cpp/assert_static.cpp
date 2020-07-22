/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-22 21:34:48
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 22:11:56
 */
#include <inttypes.h>
#include <sys/time.h>
#include <sys/types.h>

#include <boost/operators.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

class Timestamp : public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp> {
 public:
  Timestamp() : microSecondsSinceEpoch_(0){};

  Timestamp(int64_t microSecondsSinceEpoch)
      : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

  void swap(Timestamp& other) {
    std::swap(microSecondsSinceEpoch_, other.microSecondsSinceEpoch_);
  }

  std::string toString() const;

  std::string toFormattedString(bool showmicrosecond = true) const;

  int64_t valid() const { return microSecondsSinceEpoch_; }

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

  time_t secondsSinceEpoch() const {
    return static_cast<time_t>(microSecondsSinceEpoch_ /
                               kMicroSecondsPerSecond);
  }

  static Timestamp now();

  static Timestamp invalid() { return Timestamp{}; }

  static Timestamp fromUnixTime(time_t t) { return fromUnixTime(t, 0); }

  static Timestamp fromUnixTime(time_t t, int microseconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond +
                     microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

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
             (tm_time.tm_hour) + 8, tm_time.tm_min, tm_time.tm_sec,
             microseconds);

  } else {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             (tm_time.tm_hour) + 8, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

Timestamp Timestamp::now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

int main() {
  static_assert(sizeof(Timestamp) == sizeof(int64_t));
  Timestamp ts = Timestamp::now();
  std::cout << "int64_t microSecondsSinceEpoch = "
            << ts.microSecondsSinceEpoch() << std::endl;
  std::cout << "string = " << ts.toString() << std::endl;
  std::cout << "Formattedstring = " << ts.toFormattedString() << std::endl;

  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t now_c =
      std::chrono::system_clock::to_time_t(now /* - std::chrono::hours(24) */);
  std::cout << "24 hours ago, the time was "
            << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
  return 0;
}