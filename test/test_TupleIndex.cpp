/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-11 18:16:09
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-11 21:11:08
 */

#include <iostream>
#include <tuple>

template <typename T, typename Tuple>
struct TupleIndex;
// exit
template <typename T, typename... Types>
struct TupleIndex<T, std::tuple<T, Types...>> {
  static constexpr const std::size_t value = 0;
};
// entrance
template <typename T, typename U, typename... Types>
struct TupleIndex<T, std::tuple<U, Types...>> {
  static constexpr const std::size_t value =
      1 + TupleIndex<T, std::tuple<Types...>>::value;
};

int main() {
  typedef std::tuple<char, uint32_t, uint64_t, int32_t, int64_t, int,
                     unsigned int, long int, long long int, double, char*>
      SupportedTypes;

  SupportedTypes supportedtypes;
  size_t type_id = TupleIndex<int, SupportedTypes>::value;
  std::cout << typeid(int).name() << " sizeof = " << sizeof(int)
            << typeid(int32_t).name() << " sizeof = " << sizeof(int32_t)
            << std::endl;
  std::cout << type_id << std::endl;

  // std::tuple<int, const char*, double> tuple
  // = std::make_tuple(1,"test", 3.14);

  // just return object not type index
  auto tuple = std::make_tuple(1, "test", 3.14);
  std::cout << std::get<const char*>(tuple) << std::endl;
  std::cout << std::get<1>(tuple) << std::endl;

  return 0;
}