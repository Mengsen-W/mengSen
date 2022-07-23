/*
 * @Date: 2022-02-11 23:06:15
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-12 00:58:27
 */

#include "EventLoop.h"

#include <poll.h>

#include <cassert>
#include <iostream>
pid_t tid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

thread_local EventLoop* t_loopInThisThread = nullptr;

EventLoop::EventLoop() : looping_(false), threadId_(tid()) {
  if (t_loopInThisThread) {
    std::cout << "Another EventLoop " << t_loopInThisThread
              << " exists in this thread " << threadId_ << std::endl;
  } else {
    t_loopInThisThread = this;
  }
}

EventLoop::~EventLoop() {
  assert(!looping_);
  t_loopInThisThread = nullptr;
}

void EventLoop::abortNotInLoopThread() {
  std::cout << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << " , current thread id = " << tid() << std::endl;
  abort();
}

void EventLoop::loop() {
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  ::poll(nullptr, 0, 5 * 1000);
  std::cout << "EventLoop " << this << " start looping" << std::endl;
  looping_ = false;
}