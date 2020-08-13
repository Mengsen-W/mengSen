
#include <chrono>
#include <iostream>

uint64_t timestamp_now() {
  // cast time to microseconds
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             // get now time interger from since epoch
             std::chrono::high_resolution_clock::now().time_since_epoch())
      // convert to int64_t type
      .count();
}

/**
 * @brief format time stamp like [YYYY-MM-DD hh:mm:ss.ssss]
 * @param std::ostream& os,[save formate log time]
 * @param uint64_t timestamp [time stamp]
 * @return: void
 */
void format_timestamp(std::ostream& os, uint64_t timestamp) {
  // all convert for time_t and CMT time
  // convert microseconds duration type form int64_t
  std::chrono::nanoseconds duration{timestamp};
  // convert duration time to time_point
  std::chrono::high_resolution_clock::time_point time_point{duration};
  // convert time_point to time_t
  std::time_t time_now =
      std::chrono::high_resolution_clock::to_time_t(time_point);
  // get GMT time from time_t
  tm* gmtime = std::gmtime(&time_now);

  // save format style time
  char buffer[32];
  // converts the time format to the format we want
  strftime(buffer, 32, "%Y-%m-%d %T.", gmtime);
  char microseconds[7];
  sprintf(microseconds, "%06lu", timestamp % 1000000);
  os << '[' << buffer << microseconds << ']' << std::endl;
}

void test_time() {
  uint64_t time_now = timestamp_now();
  std::cout << time_now << std::endl;
  std::ostream& os = std::cout;
  format_timestamp(os, time_now);
}

void test() {
  int64_t timestamp =
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::high_resolution_clock::now().time_since_epoch())
          .count();
  std::cout << timestamp << std::endl;
  std::chrono::high_resolution_clock::time_point tpN{
      std::chrono::high_resolution_clock::now()};

  // time_t time_now = std::chrono::system_clock::to_time_t(tpN);
  time_t time_now = std::chrono::high_resolution_clock::to_time_t(tpN);
  tm* gmtime = std::gmtime(&time_now);

  // save format style time
  char buffer[21];
  // converts the time format to the format we want
  strftime(buffer, 32, "%Y-%m-%d %T.", gmtime);
  char microseconds[10];
  sprintf(microseconds, "%09lu", timestamp % 1000000000);
  std::cout << '[' << buffer << microseconds << ']' << std::endl;
}

void now(uint64_t n, std::ostream& os) {
  n += 3600000000000UL * 8U;
  format_timestamp(os, n);
}

int main() {
  std::cout << timestamp_now() << std::endl;
  uint64_t timestamp = timestamp_now();
  timestamp += 3600000000000UL * 8U;
  format_timestamp(std::cout, timestamp);
  // test();
  now(timestamp, std::cout);
  return 0;
}