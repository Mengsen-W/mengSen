/*
 * @Date: 2020-08-14 14:44:26
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 00:24:36
 * @FilePath: /mengsen/mengsen/base/LogFile.h
 */

#ifndef __MENGSEN_LOGFILE_H__
#define __MENGSEN_LOGFILE_H__

#include <memory>
#include <mutex>

#include "mengsen/base/NonCopyable.h"
#include "mengsen/base/Types.h"

namespace mengsen {
extern int g_logTimeZone;

namespace FileUtil {

class AppendFile;

}

class LogFile : NonCopyable {
 public:
  LogFile(const std::string& basename, off_t rollSize, bool threadSafe = true,
          int flushInterval = 3, int checkEveryN = 1024);
  ~LogFile();

  void append(const char* logline, int len);
  void flush();
  bool rollFile();

 private:
  void append_unlocked(const char* logline, int len);

  static std::string getLogFileName(const std::string& basename, time_t* now);

  const std::string basename_;
  const off_t rollSize_;
  const int flushInterval_;
  const int checkEveryN_;

  int count_;

  std::unique_ptr<std::mutex> mutex_;
  time_t startOfPeriod_;
  time_t lastRoll_;
  time_t lastFlush_;
  std::unique_ptr<FileUtil::AppendFile> file_;

  const static int kRollPerSeconds_ = 60 * 60 * 24;
};

}  // namespace mengsen

#endif  // MUDUO_BASE_LOGFILE_H
