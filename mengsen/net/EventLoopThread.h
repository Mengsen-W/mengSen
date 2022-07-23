/*
 * @Date: 2020-09-29 21:48:19
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-03 10:55:24
 * @FilePath: /mengsen/mengsen/net/EventLoopThread.h
 */

#ifndef __MENGSEN_NET_EVENTLOOPTHREAD_H__
#define __MENGSEN_NET_EVENTLOOPTHREAD_H__

#include "mengsen/base/Condition.h"
#include "mengsen/base/Mutex.h"
#include "mengsen/base/Thread.h"

namespace mengsen {
namespace net {

class EventLoop;

class EventLoopThread : NonCopyable {
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                  const std::string& name = "");
  ~EventLoopThread();

  /**
   * @brief: start eventloop in other thread
   */
  EventLoop* startLoop();

 private:
  void threadFunc();

  EventLoop* loop_ GUARDED_BY(mutex_);
  bool exiting_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_ GUARDED_BY(mutex_);
  ThreadInitCallback callback_;
};

}  // namespace net
}  // namespace mengsen

#endif  // __MENGSEN_NET_EVENTLOOPTHREAD_H__