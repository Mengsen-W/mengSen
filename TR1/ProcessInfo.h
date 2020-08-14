/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-14 15:01:40
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-14 15:11:30
 */

#ifndef __MENGSEN_PROCESSINFO_H__
#define __MENGSEN_PROCESSINFO_H__

#include <sys/types.h>

#include <vector>

#include "StringPiece.h"
#include "Timestamp.h"
#include "Types.h"

namespace mengsen {

namespace ProcessInfo {

pid_t pid();
std::string pidString();
uid_t uid();
std::string username();
uid_t euid();
int64_t startTime();
int clockTicksPerSecond();
int pageSize();
bool isDebugBuild();  // constexpr

std::string hostname();
std::string procname();
StringPiece procname(const std::string& stat);

/// read /proc/self/status
std::string procStatus();

/// read /proc/self/stat
std::string procStat();

/// read /proc/self/task/tid/stat
std::string threadStat();

/// readlink /proc/self/exe
std::string exePath();

int openedFiles();
int maxOpenFiles();

struct CpuTime {
  double userSeconds;
  double systemSeconds;

  CpuTime() : userSeconds(0.0), systemSeconds(0.0) {}

  double total() const { return userSeconds + systemSeconds; }
};

CpuTime cpuTime();

int numThreads();
std::vector<pid_t> threads();

}  // namespace ProcessInfo

}  // namespace mengsen

#endif  // __MENGSEN_PROCESSINFO_H__
