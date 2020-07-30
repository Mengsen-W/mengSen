/*
 * @Author: Mengsen.Wang
 * @Date: 2020-06-11 18:16:09
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-06-11 21:11:08
 */

#include <iostream>
#include <tuple>

// 前向声明，必须使用两个模板参数
template <typename T, typename Tuple>
struct TupleIndex;

// exit 知道传入参数的第一个参数和 T 一致
// T 只是一个类型，这里意思是 tuple 和 的第一个参数和 T 一致

template <typename T, typename... Types>
struct TupleIndex<T, std::tuple<T, Types...>> {
  static constexpr const std::size_t value = 0;
};

// entrance 传入参数的第一个参数和 T 不一致
// 这里的偏特化是特化了 tuple
template <typename T, typename U, typename... Types>
struct TupleIndex<T, std::tuple<U, Types...>> {
  static constexpr const std::size_t value =
      1 + TupleIndex<T, std::tuple<Types...>>::value;
};

// 模板元编程不要去看 template 列表，而是直接去看本体，向前推列表

// TupleIndex<long, std::tuple<char, int>> -> TupleIndex<long, tuple<int>
// 这里 必须存在一个相等否则就无法继续构造 tuple<T, Types...>
// 当出口为  std::tuple<Types...> 时
// 是一个更基础的版本，若存在入口函数，则不会使用这个，就连tuple<> 也可以满足
// 是因为到 tuple<> 在下面那个模板中不满足了，他会先在 ...Types 中使用0个个数
// U 是空类型，则不满足，只有可变参数才能满足空参数类型
// 所以在找不到的情况下到第一个返回满足

// 而正常情况下，找不到时，由于没有模板满足，就会编译错误
// 找的到的时候，就会从上面那个返回

int main() {
  typedef std::tuple<char, int> SupportedTypes;

  SupportedTypes supportedtypes;
  // size_t type_id = TupleIndex<long, SupportedTypes>::value;
  // std::cout << "type_id = " << type_id << std::endl;
  // std::cout << "typeid(SupportedTypes) = " << typeid(SupportedTypes).name()
  //           << "size = " << sizeof(SupportedTypes) << std::endl;
  // std::cout << typeid(int).name() << " sizeof = " << sizeof(int)
  //           << typeid(int32_t).name() << " sizeof = " << sizeof(int32_t)
  //           << std::endl;

  // std::tuple<int, const char*, double> tuple
  // = std::make_tuple(1,"test", 3.14);

  // just return object not type index
  // auto tuple = std::make_tuple(1, "test", 3.14);
  // std::cout << std::get<const char*>(tuple) << std::endl;
  // std::cout << std::get<1>(tuple) << std::endl;

  return 0;
}