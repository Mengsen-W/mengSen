/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-14 18:11:45
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-14 18:21:51
 */

#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

int main() {
  pid_t pt;
  pt = gettid();
  std::cout << pt << std::endl;
  pthread_t pp;
  pp = pthread_self();
  std::cout << pp << std::endl;
  return 0;
}