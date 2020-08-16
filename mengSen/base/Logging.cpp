/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 15:25:20
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 15:57:29
 */

#include "Logging.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

#include "CurrentThread.h"
#include "TimeZone.h"
#include "Timestamp.h"

namespace mengsen {

thread_local char t_errnobuf[512];
thread_local char t_time[64];
// thread_local std::string t_lastSecond;

const char* strerror_tl(int savedErrno) {
  return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}

Logger::LogLevel initLogLevel() {
  if (::getenv("MENGSEN_LOG_TRACE"))
    return Logger::TRACE;
  else if (::getenv("MENGSEN_LOG_DEBUG"))
    return Logger::DEBUG;
  else
    return Logger::INFO;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL ",
};

// helper class for known string length at compile time
class T {
 public:
  T(const char* str, unsigned len) : str_(str), len_(len) {
    assert(strlen(str) == len_);
  }

  const char* str_;
  const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v) {
  s.append(v.str_, v.len_);
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
  s.append(v.data_, v.size_);
  return s;
}

void defaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  // FIXME check n

  // waring unused
  (void)n;
}

void defaultFlush() { fflush(stdout); }

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
int g_logTimeZone = 0;

}  // namespace mengsen

using namespace mengsen;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file,
                   int line, const char* func)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(file),
      func_(func) {
  formatTime();
  CurrentThread::tid();
  stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength())
          << ' ';
  stream_ << T(LogLevelName[level], 6);
  if (savedErrno != 0) {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::formatTime() {
  uint64_t microSecondsSinceEpoch = time_;

  if (g_logTimeZone) {
    microSecondsSinceEpoch =
        Timestamp::switch_timezone(microSecondsSinceEpoch, g_logTimeZone);
  }

  std::string microseconds =
      Timestamp::convert<uint64_t, std::string>(
          microSecondsSinceEpoch, Timestamp::Precision::microsecond) +
      ' ';

  stream_ << microseconds;
}

void Logger::Impl::finish() {
  if (func_)
    stream_ << " - " << basename_ << ':' << line_ << ':' << func_ << '\n';
  else
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line, func) {}

Logger::Logger(SourceFile file, int line, LogLevel level)
    : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line) {}

Logger::~Logger() {
  impl_.finish();
  const LogStream::Buffer& buf(stream().buffer());
  g_output(buf.data(), buf.length());
  if (impl_.level_ == FATAL) {
    g_flush();
    abort();
  }
}

void Logger::setLogLevel(Logger::LogLevel level) { g_logLevel = level; }

void Logger::setOutput(OutputFunc out) { g_output = out; }

void Logger::setFlush(FlushFunc flush) { g_flush = flush; }

void Logger::setTimeZone(int tz) { g_logTimeZone = tz; }
