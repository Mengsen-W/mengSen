/*
 * @Date: 2020-08-06 16:34:34
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-11 10:59:43
 * @FilePath: /mengsen/mengsen/base/CurrentThread.cpp
 */

#include "mengsen/base/CurrentThread.h"

#include <cxxabi.h>    // for __cxa_demangle
#include <execinfo.h>  // for backtrace_symbols
#include <unistd.h>

#include <thread>

namespace mengsen {

namespace CurrentThread {
thread_local int t_cachedTid = 0;
thread_local char t_tidString[32];
thread_local int t_tidStringLength = 6;
thread_local const char* t_threadName = "unknown";

static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

void sleepUsec(int64_t usec) {
  std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

void yield() { std::this_thread::yield(); }

bool isMainThread() {
  // init a process has only one thread the pid and the tid are always the same
  // https://stackoverflow.com/questions/4517301/difference-between-pid-and-tid
  return CurrentThread::tid() == ::getpid();
}

/**
 * @brief recorider context
 * @param demangle [in][bool]
 * @return string
 */
std::string stackTrace(bool demangle) {
  std::string stack;
  const int max_frames = 200;
  void* frame[max_frames];
  int nptrs = ::backtrace(frame, max_frames);
  char** strings = ::backtrace_symbols(frame, nptrs);

  if (strings) {
    size_t len = 256;
    char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
    for (int i = 1; i < nptrs; ++i) {
      if (demangle) {
        char* left_par = nullptr;
        char* plus = nullptr;
        for (char* p = strings[i]; *p; ++p) {
          if (*p == '(')
            left_par = p;
          else if (*p == '+')
            plus = p;
        }
        if (left_par && plus) {
          *plus = '\0';
          int status = 0;
          char* ret =
              abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
          *plus = '+';

          if (status == 0) {
            demangled = ret;
            stack.append(strings[i], left_par + 1);
            stack.append(demangled);
            stack.append(plus);
            stack.push_back('\n');
            continue;
          }
        }
      }
      stack.append(strings[i]);
      stack.push_back('\n');
    }
    free(demangled);
    free(strings);
  }
  return stack;
}

}  // namespace CurrentThread

}  // namespace mengsen