/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-14 18:26:35
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-14 21:46:40
 */

#include "CurrentThread.h"

#include <execinfo.h>

namespace mengsen {

namespace CurrentThread {

extern __thread int t_cachedTid = 0;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength = 6;
extern __thread const char* t_threadName = "unknown";

static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");

/**
 * @brief recorider context
 * @param demangle [in][bool]
 * @return string
 */
std::string stackTrace(bool demangle) {
  std::string stack;
  const int max_frames = 200;
  void* frame[max_frames];
  //  returns a backtrace for the calling program
  // in the array pointed to by buffer
  int nptrs = ::backtrace(frame, max_frames);
  // translates the addresses into an array of strings
  // that describe the addresses symbolically
  char** strings = ::backtrace_symbols(frame, nptrs);

  if (strings) {
    size_t len = 256;
    char* demangled = demangle ? static_cast<char*>(::malloc(len)) : nullptr;
    for (int i = 1; i < nptrs; ++i) {
      // skipping the 0-th, which is this function

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
