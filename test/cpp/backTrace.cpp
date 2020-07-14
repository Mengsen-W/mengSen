/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-14 21:28:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-14 22:16:11
 */

#include <cxxabi.h>
#include <execinfo.h>

#include <iostream>

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
          if (*p == '(') {
            left_par = p;
          } else if (*p == '+') {
            plus = p;
          }
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

void test2() {
  std::string res;
  res = stackTrace(1);
  std::cout << res << std::endl;
  return;
}

void test1(int a) {
  ++a;
  test2();
  return;
}

int main() {
  test1(1);
  return 0;
}