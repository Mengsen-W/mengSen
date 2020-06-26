/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-06 19:28:46
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-06 20:34:08
 */

#include <cstring>
#include <iostream>

struct string_literal_t {
  explicit string_literal_t(const char *s) : _s(s) {}
  const char *_s;
};

int main() {
  const char *a = "wms";
  string_literal_t sa(a);
  std::cout << strlen(a);
  std::cout << sizeof(*a);
  std::cout << sa._s;
  std::cout << strlen(sa._s);
  std::cout << sizeof(*(sa._s));
  return 0;
}