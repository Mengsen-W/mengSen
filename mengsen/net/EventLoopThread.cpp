/*
 * @Date: 2022-02-21 10:25:37
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-02 17:04:49
 * @FilePath: /mengsen/mengsen/net/EventLoopThread.cpp
 */

#include "mengsen/net/EventLoopThread.h"

#include "mengsen/net/EventLoop.h"

namespace mengsen {

namespace net {
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const std::string& name)
    : loop_(NULL),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this), name),
      mutex_(),
      cond_(mutex_),
      callback_(cb) {}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  if (loop_ != NULL) {
    // not 100% race-free, eg. threadFunc could be running callback_.
    // in this time loop is NULL, so loop running always and orphan thread do
    // not join

    // if threadFunc crack and return from threadFunc, then loop unknow pointer
    // value because loop is stack pointer. So if loop is NULL that thread is
    // orphan.

    // still a tiny chance to call destructed object, if threadFunc
    // exits just now. but when EventLoopThread destructs, usually programming
    // is exiting anyway.
    loop_->quit();
    thread_.join();
  }
}

EventLoop* EventLoopThread::startLoop() {
  assert(!thread_.started());
  thread_.start();

  EventLoop* loop = NULL;
  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL) {
      cond_.wait();
    }
    loop = loop_;
  }

  return loop;
}

void EventLoopThread::threadFunc() {
  EventLoop loop;

  if (callback_) {
    callback_(&loop);
  }

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
  // assert(exiting_);
  MutexLockGuard lock(mutex_);
  loop_ = NULL;
}

}  // namespace net

}  // namespace mengsen
