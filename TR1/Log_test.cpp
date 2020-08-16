/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-15 15:03:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 21:36:01
 */

#include <unistd.h>

#include <iostream>

#include "AsyncLogging.h"
#include "LogFile.h"
#include "Logging.h"

std::unique_ptr<mengsen::LogFile> g_LogFile;
std::unique_ptr<mengsen::AsyncLogging> g_AsyncLog;

void outputFunc(const char* msg, int len) { g_LogFile->append(msg, len); }
void AsyncLogputFun(const char* msg, int len) { g_AsyncLog->append(msg, len); }

void flushFunc() { g_LogFile->flush(); }

void test_LogFile() {
  mengsen::Logger::setOutput(outputFunc);
  mengsen::Logger::setFlush(flushFunc);
  mengsen::Logger::setLogLevel(mengsen::Logger::DEBUG);
  mengsen::Logger::setTimeZone(8);

  g_LogFile.reset(
      new mengsen::LogFile("/home/Mengsen/mengSen/logfile/test", 200 * 1000));

  std::string line = "Hello World";

  for (int i = 0; i < 100000; ++i) {
    LOG_INFO << "HelloWorld" << i;
  }
}

void test_Async() {
  mengsen::Logger::setOutput(AsyncLogputFun);
  mengsen::Logger::setLogLevel(mengsen::Logger::DEBUG);
  g_AsyncLog.reset(new mengsen::AsyncLogging(
      "/home/Mengsen/mengSen/logfile/test", 200 * 1000));
  g_AsyncLog->start();
  for (int i = 0; i < 100000; ++i) {
    LOG_DEBUG << "HelloWorld" << i;
  }
}

int main(int argc, char* argv[]) {
  test_LogFile();
  std::cout << "Async log" << std::endl;
  // test_Async();

  // sleep(10);

  return 0;
}