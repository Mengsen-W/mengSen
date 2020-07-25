/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-22 17:57:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 10:38:37
 */

#ifndef __MENGSEN_TIMESTAMP_H__
#define __MENGSEN_TIMESTAMP_H__

// #include <boost/operators.hpp>
#include <string>

/**
 * @brief provide timestamp
 */
class Timestamp /* : public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp> */
{
 public:
  /**
   * @brief constructor an invalid timestamp
   */
  Timestamp() : microSecondsSinceEpoch_(0){};

  /**
   * @brief constructor an int64_t
   * @param microSecondsSinceEpoch [int64_t][in]
   */
  Timestamp(int64_t microSecondsSinceEpoch)
      : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

  void swap(Timestamp& other) {
    std::swap(microSecondsSinceEpoch_, other.microSecondsSinceEpoch_);
  }

  /**
   * @param void
   * @return [std::string] no formatted string
   */
  std::string toString() const;

  /**
   * @brief default show microsecond = true
   * @param showmicrosecond = true [bool]
   * @return [std::string] formatted string
   */
  std::string toFormattedString(bool showmicrosecond = true) const;

  /**
   * @return [int64_t] return microSeconds since Epoch to int64_t
   */
  int64_t valid() const { return microSecondsSinceEpoch_; }

  /**
   * @retrun [int64_t]  microSeconds since epoch
   */
  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  /**
   * @return [time_t] seconds since epoch
   */
  time_t secondsSinceEpoch() const {
    return static_cast<time_t>(microSecondsSinceEpoch_ /
                               kMicroSecondsPerSecond);
  }

  /**
   * @brief `static function` get now Timestamp Object
   * @return Timestamp
   */
  static Timestamp now();

  /**
   * @brief `static function` get a black Timestamp
   * @param void
   * @return [Timestamp]
   */
  static Timestamp invalid() { return Timestamp{}; }

  /**
   * @brief `static function` get Timestamp from Unix [time_t] t
   * @param t [time_t]
   * @return [Timestamp]
   */
  static Timestamp fromUnixTime(time_t t) { return fromUnixTime(t, 0); }

  /**
   * @brief `static function` get Timestamp from unix [time_t] t and [int]
   * microseconds
   * @param t [time_t]
   * @param microseconds [int]
   */
  static Timestamp fromUnixTime(time_t t, int microseconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond +
                     microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp& lhs, Timestamp& rhs) {
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp& lhs, Timestamp& rhs) {
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

/**
 * @brief Get time difference of two timestamps, result in seconds
 * @param high [const Timestamp &]
 * @param low [const Timestamp &]
 * @return [double] (high - low) in seconds
 * @c double has 52-bit precision, enough for one-microsecond resolution for
 * next 100 years
 */
inline double timeDifference(const Timestamp& lhs, const Timestamp& rhs) {
  int64_t diff = lhs.microSecondsSinceEpoch() - rhs.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

/**
 * @brief add seconds to given timestamp
 * @param timestamp [Timestamp]
 * @param second [double]
 * @return [Timestamp]
 */
inline Timestamp addTime(const Timestamp& timestamp, double seconds) {
  int64_t delta =
      static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

#endif  // __MENGSEN_TIMESTAMP_H__