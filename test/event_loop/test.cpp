/*
 * @Date: 2022-02-11 23:32:31
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-12 00:58:07
 */

#include <pthread.h>

#include <iostream>

#include "EventLoop.h"

void* positive_thread_func(void* arg) {
  std::cout << "positive_thread_func() pid = " << getpid()
            << " thread id = " << tid() << std::endl;
  EventLoop loop;
  loop.loop();
  return nullptr;
}

void positive_case() {
  std::cout << "positive_case" << std::endl;
  std::cout << "main process id = " << getpid() << " thread id = " << tid()
            << std::endl;
  EventLoop loop;
  pthread_t t;
  pthread_create(&t, nullptr, positive_thread_func, nullptr);
  sleep(1);
  loop.loop();
  pthread_join(t, nullptr);
}

EventLoop* g_loop = nullptr;

void* negative_thread_func(void* arg) {
  std::cout << "thread_func() pid = " << getpid() << " thread id = " << tid()
            << std::endl;
  g_loop->loop();
  return nullptr;
}

void negative_case() {
  std::cout << "negative_case" << std::endl;
  std::cout << "main process id = " << getpid() << " thread id = " << tid()
            << std::endl;
  EventLoop loop;
  g_loop = &loop;
  pthread_t t;
  pthread_create(&t, nullptr, negative_thread_func, nullptr);
  sleep(1);
  loop.loop();
  pthread_join(t, nullptr);
}

int main() {
  positive_case();
  negative_case();
}