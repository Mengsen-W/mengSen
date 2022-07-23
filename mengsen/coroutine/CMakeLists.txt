set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}/coroutine)

set(SOURCE
  schedule.cpp
  coroutine.cpp
)

add_library(mengsen_co STATIC ${SOURCE})