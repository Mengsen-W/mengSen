/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-06 22:24:55
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-14 16:16:31
 */

#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "CurrentThread.h"
#include "Log.h"
#include "LogFile.h"
#include "Logging.h"
#include "Thread.h"
#include "ThreadPool.h"
#include "TimeZone.h"

// pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void function() {
  LOG_DEBUG << mengsen::CurrentThread::tidString() << " "
            << mengsen::CurrentThread::name() << " "
            << "test log";
}

void test_thread() {
  std::string name = "wms";
  for (int i = 0; i < 10; i++) {
    mengsen::Thread(function, name).start();
  }

  std::cout << "finish start" << std::endl;
}

mengsen::ThreadPool tp;

void test_ThreadPool() {
  for (int i = 0; i < 10000000; i++) tp.run(function);
  tp.start(std::thread::hardware_concurrency() + 2);
}

void test_mengsen_log() {
  mengsen::log::initialize(mengsen::NonGuaranteedLogger(105), "/dev/null/",
                           "mengsen", 10);
  LOG_DEBUG << mengsen::CurrentThread::tidString() << " "
            << mengsen::CurrentThread::name() << " "
            << "test log";
  // test_thread();
  test_ThreadPool();

  while (tp.queueSize() != 0) mengsen::CurrentThread::yield();
}

int g_total;
FILE* g_file;
std::unique_ptr<mengsen::LogFile> g_logFile;

void dummyOutput(const char* msg, int len) {
  g_total += len;
  if (g_file) {
    fwrite(msg, 1, len, g_file);
  } else if (g_logFile) {
    g_logFile->append(msg, len);
  }
}

void bench(const char* type) {
  mengsen::Logger::setOutput(dummyOutput);
  int64_t start(mengsen::Timestamp::now());
  g_total = 0;

  int n = 1000 * 1000;
  const bool kLongLog = false;
  std::string empty = " ";
  std::string longStr(3000, 'X');
  longStr += " ";
  for (int i = 0; i < n; ++i) {
    MUDUO_LOG_INFO << "Hello 0123456789"
                   << " abcdefghijklmnopqrstuvwxyz"
                   << (kLongLog ? longStr : empty) << i;
  }
  int64_t end(mengsen::Timestamp::now());
  double seconds = static_cast<double>(end - start);
  printf("%12s: %f seconds, %d bytes, %10.2f msg/s, %.2f MiB/s\n", type,
         seconds, g_total, n / seconds, g_total / seconds / (1024 * 1024));
}

void logInThread() {
  MUDUO_LOG_INFO << mengsen::CurrentThread::tidString() << " "
                 << mengsen::CurrentThread::name() << " "
                 << "test log";
}

void test_muduo_log() {
  getppid();  // for ltrace and strace

  // mengsen::ThreadPool pool("pool");
  // pool.start(4);
  // pool.run(logInThread);
  // pool.run(logInThread);
  // pool.run(logInThread);
  // pool.run(logInThread);

  MUDUO_LOG_TRACE << "trace";
  MUDUO_LOG_DEBUG << "debug";
  MUDUO_LOG_INFO << "Hello";
  MUDUO_LOG_WARN << "World";
  MUDUO_LOG_ERROR << "Error";
  MUDUO_LOG_INFO << sizeof(mengsen::Logger);
  MUDUO_LOG_INFO << sizeof(mengsen::LogStream);
  MUDUO_LOG_INFO << sizeof(mengsen::Fmt);
  MUDUO_LOG_INFO << sizeof(mengsen::LogStream::Buffer);

  // sleep(1);
  // bench("nop");

  // char buffer[64 * 1024];

  // g_file = fopen("/dev/null", "w");
  // setbuffer(g_file, buffer, sizeof buffer);
  // bench("/dev/null");
  // fclose(g_file);

  // g_file = fopen("/tmp/log", "w");
  // setbuffer(g_file, buffer, sizeof buffer);
  // bench("/tmp/log");
  // fclose(g_file);

  // g_file = NULL;
  // g_logFile.reset(
  //     new mengsen::LogFile("test_log_st", 500 * 1000 * 1000, false));
  // bench("test_log_st");

  // g_logFile.reset(new mengsen::LogFile("test_log_mt", 500 * 1000 * 1000,
  // true)); bench("test_log_mt"); g_logFile.reset();

  // {
  //   g_file = stdout;
  //   sleep(1);
  //   mengsen::TimeZone beijing(8 * 3600, "CST");
  //   mengsen::Logger::setTimeZone(beijing);
  //   MUDUO_LOG_TRACE << "trace CST";
  //   MUDUO_LOG_DEBUG << "debug CST";
  //   MUDUO_LOG_INFO << "Hello CST";
  //   MUDUO_LOG_WARN << "World CST";
  //   MUDUO_LOG_ERROR << "Error CST";

  //   sleep(1);
  //   mengsen::TimeZone newyork("/usr/share/zoneinfo/America/New_York");
  //   mengsen::Logger::setTimeZone(newyork);
  //   MUDUO_LOG_TRACE << "trace NYT";
  //   MUDUO_LOG_DEBUG << "debug NYT";
  //   MUDUO_LOG_INFO << "Hello NYT";
  //   MUDUO_LOG_WARN << "World NYT";
  //   MUDUO_LOG_ERROR << "Error NYT";
  //   g_file = NULL;
  // }
  // bench("timezone nop");
}

int main() {
  // test_mengsen_log();
  test_muduo_log();
  return 0;
}