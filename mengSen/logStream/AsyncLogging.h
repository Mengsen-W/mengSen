/*
 * @Author: Mengsen.Wang
 * @Date: 2020-05-18 21:57:54
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-05-19 12:51:12
 * @Description: 异步的日志中间件
 */

#ifndef __MENGSEN_LOG_ASYNCLOGGING_H__
#define __MENGSEN_LOG_ASYNCLOGGING_H__

#include <atomic>
#include <vector>

#include "../types/noncopyable.h"

namespace mengSen {

class AsyncLogging : noncopyable {
 public:
  AsyncLogging();
  ~AsyncLogging();

  void append();

  void start();
  void stop();

 private:
  void threadFunc();

  const int flushInterval_;    // flush interval time
  std::atomic<bool> running_;  // bool for running
  // const std::string basename_;
};

}  // namespace mengSen

#endif  //__MENGSEN_LOG_ASYNCLOGGING_H__