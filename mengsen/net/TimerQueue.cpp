/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-31 21:14:36
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-09-09 21:33:56
 */

#ifdef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "mengsen/net/TimerQueue.h"

#include <sys/timerfd.h>
#include <unistd.h>

#include "mengsen/base/Logging.h"
#include "mengsen/net/EventLoop.h"
#include "mengsen/net/Timer.h"
#include "mengsen/net/TimerId.h"

namespace mengsen {

namespace net {

namespace detail {

/**
 * @brief create Timer fd and error handling
 * @return timerfd [int]
 */
int createTimerfd() {
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);

  if (timerfd < 0) {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }

  return timerfd;
}

/**
 * @brief get how mach time from input parameter when to now
 * @param when [Timestamp::Timestamp::Time_uint64_t]
 * @return [struct timespec]
 */
struct timespec howMuchTimeFromNow(Timestamp::Time_uint64_t when) {
  Timestamp::Time_uint64_t microseconds =
      when - Timestamp::now<Timestamp::Time_uint64_t>();
  if (microseconds < 100) {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec =
      static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

/**
 * @brief read Timer fd
 * @param timerfd [int]
 * @param now [Timestamp::Time_uint64_t]
 */
void readTimerfd(int timerfd, Timestamp::Time_uint64_t now) {
  Timestamp::Time_uint64_t howmany;
  // 'howmany' is Number of unprocessed maturities
  ssize_t n = ::read(timerfd, &howmany, sizeof(howmany));
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at "
            << Timestamp::convert<Timestamp::Time_uint64_t, std::string>(now);
  if (n != sizeof(howmany)) {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n
              << " bytes instead of 8";
  }
}

/**
 * @brief reset timerfd just call system call timerfd_settime()
 * @param timerfd [int]
 * @param expiration [Timestamp::Time_uint64_t] time point
 */
void resetTimerfd(int timerfd, Timestamp::Time_uint64_t expiration) {
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  memZero(&newValue, sizeof(newValue));
  memZero(&oldValue, sizeof(oldValue));
  newValue.it_value = howMuchTimeFromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret) {
    LOG_SYSERR << "timerfd_settime()";
  }
}

}  // namespace detail

TimerQueue::TimerQueue(EventLoop* loop)
    : loop_(loop),
      timerfd_(detail::createTimerfd()),
      timerfdChannel_(loop, timerfd_),
      timers_(),
      callingExpiredTimers_(false) {
  timerfdChannel_.setReadCallback(std::bind(&TimerQueue::handleRead, this));
  timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue() {
  timerfdChannel_.disableAll();
  timerfdChannel_.remove();
  ::close(timerfd_);
  // do not remove channel, since we're in EventLoop::dtor();
  for (const Entry& timer : timers_) {
    delete timer.second;
  }
}

TimerId TimerQueue::addTimer(TimerCallback cb, Timestamp::Time_uint64_t when,
                             double interval) {
  Timer* timer = new Timer(std::move(cb), when, interval);
  loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId{timer, timer->sequence()};
}

void TimerQueue::cancel(TimerId timerId) {
  loop_->runInLoop(std::bind(&TimerQueue::cancelInLoop, this, timerId));
}

void TimerQueue::addTimerInLoop(Timer* timer) {
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if (earliestChanged) {
    // update earliest
    detail::resetTimerfd(timerfd_, timer->expiration());
  }
}

void TimerQueue::cancelInLoop(TimerId timerId) {
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_, timerId.sequence_);
  ActiveTimerSet::iterator it = activeTimers_.find(timer);

  if (it != activeTimers_.end()) {
    size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1);
    (void)n;
    delete it->first;
    activeTimers_.erase(it);
  } else if (callingExpiredTimers_) {
    // timer callback function call cancel() this timer means self cannel
    cancelingTimers_.insert(timer);
  }
  assert(timers_.size() == activeTimers_.size());
}

void TimerQueue::handleRead() {
  loop_->assertInLoopThread();
  Timestamp::Time_uint64_t now = Timestamp::now<Timestamp::Time_uint64_t>();
  detail::readTimerfd(timerfd_, now);

  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();

  for (const Entry& it : expired) {
    it.second->run();
  }

  callingExpiredTimers_ = false;

  reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(
    Timestamp::Time_uint64_t now) {
  assert(timers_.size() == activeTimers_.size());

  std::vector<Entry> expired;

  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));

  // end time >= now
  // if not found return last iterator
  TimerList::iterator end = timers_.lower_bound(sentry);

  // true || true => true all elements expired
  // true || false => true all elements expired
  // false || true => true inner some element not expired
  // false || false => false inner element and time > now has error
  assert(end == timers_.end() || now < end->first);

  std::copy(timers_.begin(), end, back_inserter(expired));
  timers_.erase(timers_.begin(), end);

  for (const Entry& it : expired) {
    ActiveTimer timer(it.second, it.second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1);
    (void)n;
  }

  assert(timers_.size() == activeTimers_.size());
  return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired,
                       Timestamp::Time_uint64_t now) {
  Timestamp::Time_uint64_t nextExpire;

  for (const Entry& it : expired) {
    ActiveTimer timer(it.second, it.second->sequence());
    if (it.second->repeat() &&
        cancelingTimers_.find(timer) == cancelingTimers_.end()) {
      // 1. timer is repeat
      // 2. timer is not canceled

      // reset timer and instert it into timer queue
      it.second->restart(now);
      insert(it.second);
    } else {
      // FIXME move to a free list
      // FIXME: no delete please
      delete it.second;
    }
  }

  if (!timers_.empty()) {
    nextExpire = timers_.begin()->second->expiration();
  }

  if (Timestamp::valid(nextExpire)) {
    detail::resetTimerfd(timerfd_, nextExpire);
  }
}

bool TimerQueue::insert(Timer* timer) {
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  bool earliestChanged = false;
  Timestamp::Time_uint64_t when = timer->expiration();
  TimerList::iterator it = timers_.begin();
  // if set is empty or timer's expiration is earlier than the first timer
  if (it == timers_.end() || when < it->first) {
    earliestChanged = true;
  }
  {
    std::pair<TimerList::iterator, bool> result =
        timers_.insert(Entry(when, timer));
    assert(result.second);
    (void)result;
  }
  {
    std::pair<ActiveTimerSet::iterator, bool> result =
        activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
    assert(result.second);
    (void)result;
  }

  assert(timers_.size() == activeTimers_.size());
  return earliestChanged;
}

}  // namespace net

}  // namespace mengsen