/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 11:55:15
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-14 17:45:10
 */

#ifndef __MENGSEN_LOGGING_H__
#define __MENGSEN_LOGGING_H__

#include "LogStream.h"
#include "Timestamp.h"

namespace mengsen {

class TimeZone;

class Logger {
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  // compile time calculation of basename of source file
  class SourceFile {
   public:
    template <int N>
    SourceFile(const char (&arr)[N]) : data_(arr), size_(N - 1) {
      const char* slash = strrchr(data_, '/');  // builtin function
      if (slash) {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename) : data_(filename) {
      const char* slash = strrchr(filename, '/');
      if (slash) {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& stream() { return impl_.stream_; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);
  static void setTimeZone(const TimeZone& tz);

 private:
  class Impl {
   public:
    typedef Logger::LogLevel LogLevel;
    Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
    void formatTime();
    void finish();

    uint64_t time_;
    LogStream stream_;
    LogLevel level_;
    int line_;
    SourceFile basename_;
  };

  Impl impl_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel() { return g_logLevel; }

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define MUDUO_LOG_TRACE                                      \
  if (mengsen::Logger::logLevel() <= mengsen::Logger::TRACE) \
  mengsen::Logger(__FILE__, __LINE__, mengsen::Logger::TRACE, __func__).stream()
#define MUDUO_LOG_DEBUG                                      \
  if (mengsen::Logger::logLevel() <= mengsen::Logger::DEBUG) \
  mengsen::Logger(__FILE__, __LINE__, mengsen::Logger::DEBUG, __func__).stream()
#define MUDUO_LOG_INFO                                      \
  if (mengsen::Logger::logLevel() <= mengsen::Logger::INFO) \
  mengsen::Logger(__FILE__, __LINE__).stream()
#define MUDUO_LOG_WARN \
  mengsen::Logger(__FILE__, __LINE__, mengsen::Logger::WARN).stream()
#define MUDUO_LOG_ERROR \
  mengsen::Logger(__FILE__, __LINE__, mengsen::Logger::ERROR).stream()
#define MUDUO_LOG_FATAL \
  mengsen::Logger(__FILE__, __LINE__, mengsen::Logger::FATAL).stream()
#define MUDUO_LOG_SYSERR mengsen::Logger(__FILE__, __LINE__, false).stream()
#define MUDUO_LOG_SYSFATAL mengsen::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val)                                                   \
  ::mengsen::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", \
                          (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr) {
  if (ptr == NULL) {
    Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

}  // namespace mengsen

#endif  // __MENGSEN_BASE_LOGGING_H__
