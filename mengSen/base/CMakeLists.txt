aux_source_directory(. SRC_LIST)
include_directories(${PROJECT_SOURCE_DIR})
message(STATUS ${SRC_LIST})

add_library(mengsen_base ${SRC_LIST})
target_link_libraries(mengsen_base
  PUBLIC
    Threads::Threads
)