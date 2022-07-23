/*
 * @Date: 2022-02-11 22:58:24
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-02-12 00:51:54
 * @FilePath: /mengsen/test/event_loop/EventLoop.h
 */

#include <sys/syscall.h>

#include <atomic>
#include <boost/noncopyable.hpp>

#ifndef __TEST_EVENT_LOOP_H_
#define __TEST_EVENT_LOOP_H_

pid_t tid();

class EventLoop : boost::noncopyable {
 public:
  EventLoop();
  ~EventLoop();

  void loop();

  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }

  bool isInLoopThread() const { return threadId_ == tid(); }

 private:
  void abortNotInLoopThread();
  std::atomic_bool looping_;
  const pid_t threadId_;
};

#endif  // __TEST_EVENT_LOOP_H_