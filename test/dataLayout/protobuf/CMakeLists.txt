cmake_minimum_required (VERSION 2.8)

project (Demo1)

SET(SRC_LIST
main.cpp
addressbook.pb.cc
)

# Find required protobuf package
find_package(Protobuf REQUIRED)

if(PROTOBUF_FOUND)
   message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

message(STATUS "Protobuf_VERSION ${Protobuf_VERSION}")
message(STATUS "PROTOBUF_VERSION ${PROTOBUF_VERSION}")
message(STATUS "PROTOBUF_INCLUDE_DIR ${PROTOBUF_INCLUDE_DIR}")
message(STATUS "PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS}")
message(STATUS "PROTOBUF_LIBRARY ${PROTOBUF_LIBRARY}")
message(STATUS "PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES}")

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS addressbook.proto)

add_executable(main.out ${SRC_LIST} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(main.out ${PROTOBUF_LIBRARIES})