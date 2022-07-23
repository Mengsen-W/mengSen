cmake_minimum_required( VERSION 3.10 )

project("mengsen")

enable_testing()

set(CMAKE_BUILD_TYPE Debug)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CXX_FLAGS
# for gdb
  -g

 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64

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

  -std=c++20
  -rdynamic
)

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNODEBUG")
# source dir mean CMakeLists.txt dir
# binary dir mean executed cmake dir
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)

find_package(Boost REQUIRED)

find_package(Protobuf REQUIRED)

# cmake perfer pthread library on system with multiple thread libraries available
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
# prefer -pthread compiler and linker flag
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

# find_library(YAML NAMES yaml-cpp)
# find_library(JSON NAMES jsoncpp )

# if(YAML)
#   message(STATUS "found yaml-cpp")
# endif()

# if(JSON)
#   message(STATUS "found jsoncpp")
# endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(mengsen/base)
add_subdirectory(mengsen/net)

# add_executable(main.out
  # main.cpp
# )

# target_link_libraries(main.out
  # PRIVATE
    # mengsen_co
    # mengsen_log
    # jsoncpp
    # yaml-cpp
# )
