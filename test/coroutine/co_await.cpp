/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-25 15:37:12
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-25 17:14:55
 */

#define __cpp_impl_coroutine 1

#include <coroutine>
#include <iostream>

struct wait_some_times {
  int left_times;
  std::coroutine_handle<> handle;
  wait_some_times(int t) : left_times(t), handle(nullptr) {}
};

struct suspend_some_times {
  wait_some_times& d;
  suspend_some_times(wait_some_times& _d) : d(_d) {}
  // 每次调用co_wait 会判断 await_ready
  // return true mean done call resume else call suspend
  bool await_ready() noexcept {
    std::cout << "call await_ready: " << d.left_times << std::endl;
    return d.left_times <= 0;
  }

  // 若 ready return false 则 suspend
  void await_suspend(std::coroutine_handle<> h) noexcept {
    // 记下来handle以便后面resume用
    d.handle = h;
    std::cout << "call await_suspend: " << (--d.left_times) << std::endl;
  }

  // main function call
  void await_resume() noexcept {
    std::cout << "call await_resume: " << d.left_times << std::endl;
    d.handle = nullptr;
  }
};

struct coroutine_task {
  struct promise_type {
    coroutine_task get_return_object() { return coroutine_task{}; }
    auto initial_suspend() { return std::suspend_never{}; }
    auto final_suspend() { return std::suspend_never{}; }

    void unhandled_exception() {}
    void return_void() {}
  };
};

auto operator co_await(wait_some_times& x) noexcept {
  return suspend_some_times{x};
}

coroutine_task f(wait_some_times& waiter) {
  std::cout << "begin to co_await" << std::endl;
  co_await waiter;  // 只有前三次会协程切出
  co_await waiter;
  co_await waiter;
  co_await waiter;  // 这之后await_ready返回true了，不会再切出
  co_await waiter;
  std::cout << "end of corotine" << std::endl;
}

int main(int argc, char* argv[]) {
  // left time = 3
  wait_some_times waiter{3};
  f(waiter);

  while (waiter.handle && !waiter.handle.done()) {
    std::cout << "about to resume: " << waiter.left_times << std::endl;
    // 这里用传出的handle来恢复切入协程
    waiter.handle.resume();
  }

  return 0;
}

/*
begin to co_await
call await_ready: 3
call await_suspend: 2
about to resume: 2
call await_resume: 2
call await_ready: 2
call await_suspend: 1
about to resume: 1
call await_resume: 1
call await_ready: 1
call await_suspend: 0
about to resume: 0
call await_resume: 0
call await_ready: 0
call await_resume: 0
call await_ready: 0
call await_resume: 0
end of corotine
*/