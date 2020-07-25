cmake_minimum_required(VERSION 3.10)

enable_testing()

project(basic_test)

# Setup testing
enable_testing()

# set(CMAKE_BUILD_TYPE Debug)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CXX_FLAGS
# for gdb
  -g
# for waring infomation
  -Wall
  -Wextra
  -Werror
  -Wconversion
  -Wno-unused-parameter
  -Wold-style-cast
  -Woverloaded-virtual
  -Wpointer-arith
  -Wshadow
  -Wwrite-strings
  -march=native
  -rdynamic
  -std=c++20
)

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNODEBUG")

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(GTest REQUIRED)
if(GTest_FOUND)
  include_directories(${GTEST_INCLUDE_DIR})
  message(STATUS "GTEST_INCLUDE_DIR = " ${GTEST_INCLUDE_DIR})
  message("GTEST_INCLUDE_DIRs = " ${GTEST_INCLUDE_DIRS})
  message(STATUS "GTest_Both_Libs = " ${GTEST_BOTH_LIBRARIES})
  message(STATUS "GTest_Lib = " ${GTEST_LIBRARIES})
  message(STATUS "GTest_Main_Lib = " ${GTEST_MAIN_LIBRARIES})
  message(STATUS "GTest_Root = " ${GTEST_ROOT})
else()
  message (STATUS "Not Found GoogleTest!")
endif()

find_package(Threads REQUIRED)
if(Threads_FOUND)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  set(CMAKE_USE_PTHREADS_INIT TRUE)
  set(CMAKE_USE_WIN32_THREADS_INIT FALSE)
  set(CMAKE_HP_PTHREADS_INIT FALSE)
  message(STATUS "Threads init" ${CMAKE_THREAD_LIBS_INIT})
  message(STATUS "Threads_Pthread_Flag = " ${THREADS_PREFER_PTHREAD_FLAG})
  message(STATUS "using WIN32 threads = " ${CMAKE_USE_WIN32_THREADS_INIT})
  message(STATUS "are we using pthreads = " ${CMAKE_USE_PTHREADS_INIT})
  message(STATUS "are we using hp pthreads = " ${CMAKE_HP_PTHREADS_INIT})
else()
  message (STATUS "Not Found GoogleTest!")
endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
  set(Boost_LIBRARY_DIRS "/usr/lib")
  set(Boost_LIBRARIES "/usr/lib/libboost_system.a")
  message(STATUS "Boost include dirs = " ${Boost_INCLUDE_DIRS})
  message(STATUS "Boost Link dircetions = " ${Boost_LIBRARY_DIRS})
  message(STATUS "Boost component libraries to be linked = " ${Boost_LIBRARIES})
  message(STATUS "Boost Version = " ${Boost_VERSION})
else()
  message (STATUS "Not Found Boost")
endif()

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

aux_source_directory(. SRC_LIST)
message(STATUS "SRC_LIST = " ${SRC_LIST})

add_executable(demo ${SRC_LIST})

target_link_libraries(demo
  ${GTEST_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)