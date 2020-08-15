/*
 * @Author: Mengsen.Wang
 * @Date: 2020-08-15 21:05:00
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-08-15 21:22:31
 */

#include <iostream>
#include <memory>
#include <vector>

class Buffer {
 public:
  Buffer(const char* data) : data_(data) {
    std::cout << "Constructor" << std::endl;
  }
  const char* data_;
};

void test_vec_ptrs() {
  std::vector<std::unique_ptr<Buffer>> BufferVec;
  BufferVec.reserve(8);

  for (int i = 0; i < 4; ++i) {
    BufferVec.emplace_back(
        std::move(std::unique_ptr<Buffer>(std::make_unique<Buffer>("w"))));
  }

  for (auto& b : BufferVec) {
    std::cout << b->data_ << std::endl;
  }
  std::unique_ptr<Buffer> p = std::move(BufferVec.back());
  std::cout << p->data_ << std::endl;
  BufferVec.pop_back();
  p.reset();
  std::cout << p->data_ << std::endl;

  return;
}

void test_vec() {
  std::vector<Buffer> vecBuf;

  vecBuf.reserve(8);

  for (int i = 0; i < 4; ++i) {
    vecBuf.emplace_back(std::move(Buffer("w")));
  }

  for (auto& b : vecBuf) {
    std::cout << b.data_ << std::endl;
  }

  Buffer a = std::move(vecBuf.back());
  std::cout << a.data_ << std::endl;

  // vecBuf.pop_back();

  for (auto& b : vecBuf) {
    std::cout << b.data_ << std::endl;
  }
}

int main() {
  test_vec_ptrs();
  return 0;
}