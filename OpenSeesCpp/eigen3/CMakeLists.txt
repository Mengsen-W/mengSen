cmake_minimum_required(VERSION 3.10)
project(testEigen)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_FLAGS "-O3")

include_directories("/usr/include/eigen3")

add_executable(test_eigen test_eigen.cpp)
