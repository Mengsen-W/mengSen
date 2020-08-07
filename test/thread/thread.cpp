/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-03 18:20:06
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-06 18:46:44
 */

#include <sys/syscall.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <thread>
#include <vector>

class Print {
 public:
  Print() {}
  int t;
  std::vector<int> test;
};

void print(int num = 5);

pid_t gettid() { return static_cast<pid_t>(syscall(SYS_gettid)); }

void print_pid() {
  std::cout << gettid() << std::endl;
  std::cout << std::this_thread::get_id() << std::endl;
}

void print(int num) {
  for (int i = 0; i < num; ++i) {
    sleep(1);
    std::cout << "Thread"
              << " + " << i << std::endl;
  }
}

void test() {
  Print p;
  // must be initialization for c-style type
  p.t = 1;
  std::cout << p.t << std::endl;
  p.test.emplace_back(1);
}

void test_pid() {
  print_pid();
  std::vector<std::thread> threads;
  for (int i = 0; i < 5; ++i) {
    threads.push_back(std::thread(print_pid));
  }

  for (std::thread& t : threads) {
    t.join();
  }
}

static int num = 0;
void threadTest() {
  for (int i = 1; i <= 50; ++i) num += 1;
}

void test_p() {
  std::thread t1(threadTest);
  std::thread t2(threadTest);
  t1.join();
  t2.join();
  std::cout << num << std::endl;
  num = 0;
}
void foo() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "end" << std::endl;
}

void test_joinable() {
  std::thread t;
  std::cout << "before starting, joinable: " << std::boolalpha << t.joinable()
            << '\n';

  t = std::thread(foo);
  std::cout << "after starting, joinable: " << t.joinable() << '\n';
  t.join();
  std::cout << "after joining, joinable: " << t.joinable() << '\n';
  t.join();
}

void test_detach() {
  std::thread t(foo);
  t.detach();
  std::cout << "detach" << std::endl;
}

int main() {
  // test();
  // std::vector<std::thread> threads;
  // std::cout << "begin" << std::endl;
  // threads.emplace_back(std::move(std::thread(std::bind(print, 5))));
  // threads[0].join();
  // std::cout << "join finished" << std::endl;
  // std::cout << "pid = " << ::getpid() << std::endl;
  // test_pid();
  // for (int i = 0; i < 10000; ++i) test_p();
  // test_joinable();

  return 0;
}
