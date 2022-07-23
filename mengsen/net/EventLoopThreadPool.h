/*
 * @Date: 2020-09-29 21:49:09
 * @Author: Mengsen Wang
 * @LastEditors: Mengsen Wang
 * @LastEditTime: 2022-03-07 17:16:43
 * @FilePath: /mengsen/mengsen/net/EventLoopThreadPool.h
 */

#ifndef __MENGSEN_NET_EVENTLOOPTHREADPOOL_H__
#define __MENGSEN_NET_EVENTLOOPTHREADPOOL_H__

#include <functional>
#include <memory>
#include <vector>

#include "mengsen/base/NonCopyable.h"
#include "mengsen/base/Types.h"

namespace mengsen {

namespace net {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : NonCopyable {
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
  ~EventLoopThreadPool();

  /**
   * @brief set event loop thread pool size
   * @param numThreads [int]
   */
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }

  /**
   * @brief start eventloop thread loop
   * @param cb [ThreadInitCallback&]
   */
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  /**
   * @brief get next loop, valid after calling start() and round robin return
   * @return EventLoop*
   */
  EventLoop* getNextLoop();

  ///
  /**
   * @brief get loop for hash code with the same hash code, it will always
   * return the same EventLoop
   * @param hashCode [int]
   * @return EventLoop*
   */
  EventLoop* getLoopForHash(size_t hashCode);

  /**
   * @brief get all loops
   * @return std::vector<EventLoop*>
   */
  std::vector<EventLoop*> getAllLoops();

  /**
   * @brief event loop thread pool is started
   * @return bool
   */
  bool started() const { return started_; }

  /**
   * @brief get event loop thread pool name
   * @return const std::string&
   */
  const std::string& name() const { return name_; }

 private:
  // if single thread than getNextLoop return baseLoop_ always
  EventLoop* baseLoop_;
  std::string name_;
  bool started_;
  int numThreads_;
  int next_;
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};

}  // namespace net
}  // namespace mengsen

#endif  // __MENGSEN_NET_EVENTLOOPTHREADPOOL_H__