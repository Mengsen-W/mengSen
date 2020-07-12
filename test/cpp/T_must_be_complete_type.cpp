/*
 * @Author: Mengsen.Wang
 * @Date: 2020-07-11 21:36:49
 * @Last Modified by: Mengsen.Wang
 * @Last Modified time: 2020-07-11 21:52:04
 */

#include <iostream>
#include <vector>

template <typename T>
static bool T_must_be_complate_type() {
  typedef char T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1];
  T_must_be_complate_type dummy;
  (void)dummy;
  return true;
}

class Test1 {};
class Test2;

int main() {
  std::cout << sizeof(Test1) << std::endl;
  std::cout << T_must_be_complate_type<Test1>() << std::endl;
  std::cout << sizeof(int[0]) << std::endl;
  std::cout << T_must_be_complate_type<int[0]>() << std::endl;
  std::cout << sizeof(Test2) << std::endl;
  std::cout << T_must_be_complate_type<Test2>() << std::endl;
  return 0;
}

/**
T_must_be_complete_type.cpp:27:16: error: invalid application of ‘sizeof’ to
incomplete type ‘Test2’ 27 |   std::cout << sizeof(Test2) << std::endl; |
^~~~~~~~~~~~~ T_must_be_complete_type.cpp: In instantiation of ‘bool
T_must_be_complate_type() [with T = int [0]]’:
T_must_be_complete_type.cpp:26:48:   required from here
T_must_be_complete_type.cpp:13:16: error: narrowing conversion of ‘-1’ from
‘int’ to ‘long unsigned int’ [-Wnarrowing] 13 |   typedef char
T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1]; | ^~~~~~~~~~~~~~~~~~~~~~~
T_must_be_complete_type.cpp:13:55: error: size ‘-1’ of array is negative
   13 |   typedef char T_must_be_complate_type[sizeof(T) == 0 ? -1 : 1];
      |                                        ~~~~~~~~~~~~~~~^~~~~~~~
T_must_be_complete_type.cpp: In instantiation of ‘bool T_must_be_complate_type()
[with T = Test2]’: T_must_be_complete_type.cpp:28:47:   required from here
T_must_be_complete_type.cpp:13:40: error: invalid application of ‘sizeof’ to
incomplete type ‘Test2’ 13 |   typedef char T_must_be_complate_type[sizeof(T) ==
0 ? -1 : 1];                                     ~~~~~~~~~~~~~~~^~~~~~~~
*/