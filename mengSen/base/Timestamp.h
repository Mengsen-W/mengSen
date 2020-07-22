/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-22 17:57:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-22 18:31:49
 */

#ifndef __MENGSEN_TIMESTAMP_H__
#define __MENGSEN_TIMESTAMP_H__

#include <boost/operators.hpp>

#include "copyable.h"

namespace mengsen {

/**
 * @brief provide timestamp
 */
class Timestamp : public copyable,
                  public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp> {
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
   *
   */

  static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
  int64_t microSecondsSinceEpoch_;
};

}  // namespace mengsen

#endif  // __MENGSEN_TIMESTAMP_H__