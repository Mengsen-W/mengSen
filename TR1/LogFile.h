/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 14:44:26
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 15:37:28
 */

#ifndef __MENGSEN_LOGFILE_H__
#define __MENGSEN_LOGFILE_H__

#include <memory>
#include <mutex>

#include "Types.h"
#include "noncopyable.h"

namespace mengsen {

namespace FileUtil {

class AppendFile;

}

class LogFile : noncopyable {
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
