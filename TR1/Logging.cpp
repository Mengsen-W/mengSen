/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 15:25:20
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-14 17:49:26
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

/*
class LoggerImpl
{
 public:
  typedef Logger::LogLevel LogLevel;
  LoggerImpl(LogLevel level, int old_errno, const char* file, int line);
  void finish();

  Timestamp time_;
  LogStream stream_;
  LogLevel level_;
  int line_;
  const char* fullname_;
  const char* basename_;
};
*/

thread_local char t_errnobuf[512];
thread_local char t_time[64];
thread_local std::string t_lastSecond;

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
TimeZone g_logTimeZone;

}  // namespace mengsen

using namespace mengsen;

Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file,
                   int line)
    : time_(Timestamp::now()),
      stream_(),
      level_(level),
      line_(line),
      basename_(file) {
  formatTime();
  CurrentThread::tid();
  stream_ << T(CurrentThread::tidString(), CurrentThread::tidStringLength());
  stream_ << T(LogLevelName[level], 6);
  if (savedErrno != 0) {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

void Logger::Impl::formatTime() {
  uint64_t microSecondsSinceEpoch = time_;
  std::string seconds = Timestamp::convert<uint64_t, std::string>(
      microSecondsSinceEpoch, Timestamp::Precision::second);
  std::string microseconds = Timestamp::convert<uint64_t, std::string>(
      microSecondsSinceEpoch, Timestamp::Precision::microsecond);

  // TODO change logical
  if (seconds != t_lastSecond) {
    t_lastSecond = seconds;
    struct tm tm_time;
    if (g_logTimeZone.valid()) {
      tm_time = g_logTimeZone.toLocalTime(seconds);
    } else {
      ::gmtime_r(&seconds, &tm_time);  // FIXME TimeZone::fromUtcTime
    }

    int len =
        snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17);
    (void)len;
  }

  if (g_logTimeZone.valid()) {
    Fmt us(".%06d ", microseconds);
    printf("%d", us.length());
    assert(us.length() == 10);
    stream_ << T(t_time, 17) << T(us.data(), 8);
  } else {
    Fmt us(".%06dZ ", microseconds);
    assert(us.length() == 10);
    stream_ << T(t_time, 17) << T(us.data(), 9);
  }
}

void Logger::Impl::finish() {
  stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
    : impl_(level, 0, file, line) {
  impl_.stream_ << func << ' ';
}

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

void Logger::setTimeZone(const TimeZone& tz) { g_logTimeZone = tz; }