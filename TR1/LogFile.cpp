/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 14:46:39
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 21:32:24
 */

#include "LogFile.h"

#include <stdio.h>
#include <time.h>

#include <cassert>

#include "FileUtil.h"
#include "ProcessInfo.h"

using namespace mengsen;

LogFile::LogFile(const std::string& basename, off_t rollSize, bool threadSafe,
                 int flushInterval, int checkEveryN)
    : basename_(basename),
      rollSize_(rollSize),
      flushInterval_(flushInterval),
      checkEveryN_(checkEveryN),
      count_(0),
      mutex_(threadSafe ? std::make_unique<std::mutex>() : NULL),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0) {
  // assert(basename.find('/') == std::string::npos);
  rollFile();
}

LogFile::~LogFile() = default;

void LogFile::append(const char* logline, int len) {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    append_unlocked(logline, len);
  } else {
    append_unlocked(logline, len);
  }
}

void LogFile::flush() {
  if (mutex_) {
    std::lock_guard<std::mutex> lock(*mutex_);
    file_->flush();
  } else {
    file_->flush();
  }
}

void LogFile::append_unlocked(const char* logline, int len) {
  file_->append(logline, len);

  if (file_->writtenBytes() > rollSize_) {
    rollFile();
  } else {
    ++count_;
    if (count_ >= checkEveryN_) {
      count_ = 0;
      time_t now = ::time(NULL);
      time_t thisPeriod_ = now / kRollPerSeconds_ * kRollPerSeconds_;
      if (thisPeriod_ != startOfPeriod_) {
        rollFile();
      } else if (now - lastFlush_ > flushInterval_) {
        lastFlush_ = now;
        file_->flush();
      }
    }
  }
}

bool LogFile::rollFile() {
  time_t now = 0;
  std::string filename = getLogFileName(basename_, &now);
  time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

  if (now > lastRoll_) {
    lastRoll_ = now;
    lastFlush_ = now;
    startOfPeriod_ = start;
    file_.reset(new FileUtil::AppendFile(filename));
    return true;
  }
  return false;
}

std::string LogFile::getLogFileName(const std::string& basename, time_t* now) {
  std::string filename;
  filename.reserve(basename.size() + 64);
  filename = basename;

  char timebuf[32];
  struct tm tm;
  *now = time(NULL);
  gmtime_r(now, &tm);  // FIXME: localtime_r ?
  strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
  filename += timebuf;

  filename += ProcessInfo::hostname();

  char pidbuf[32];
  snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
  filename += pidbuf;

  filename += ".log";

  return filename;
}
