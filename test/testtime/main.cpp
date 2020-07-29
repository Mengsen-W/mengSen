/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-25 09:40:54
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 11:56:33
 */

#include <climits>
#include <fstream>
#include <iostream>

#include "./include/Timestamp.h"
#include "./include/chrono_Timestamp.h"
#include "./include/simple_time.h"
#include "gtest/gtest.h"

static std::ofstream fout{"/dev/null"};

#define TEST_NUMBER 50000000

#define OUT fout

int chrono_time(void) {
  for (int64_t i = TEST_NUMBER; i > 0; --i) {
    OUT << mengsen_timestamp::toString_now(mengsen_timestamp::microsecond)
        << std::endl;
  }
  return 0;
}

int simple_time(void) {
  for (int64_t i = TEST_NUMBER; i > 0; --i) {
    format_timestamp(OUT, timestamp_now());
  }
  return 0;
}

int timstamps(void) {
  for (int64_t i = TEST_NUMBER; i > 0; --i) {
    OUT << Timestamp::now().toFormattedString() << std::endl;
  }
  return 0;
}

TEST(simple_time1, time) { EXPECT_EQ(0, simple_time()); }
TEST(chrono_time1, time) { EXPECT_EQ(0, chrono_time()); }
TEST(timstamps, time) { EXPECT_EQ(0, timstamps()); }
TEST(chrono_time2, time) { EXPECT_EQ(0, chrono_time()); }
TEST(simple_time2, time) { EXPECT_EQ(0, simple_time()); }

int main() {
  // chrono_time();
  // simple_time();
  // timstamps();
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
  // return 0;
}
