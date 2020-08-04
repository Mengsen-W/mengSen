set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}/base)

aux_source_directory(. SRC_LIST)
message(STATUS "SRC_LIST = " ${SRC_LIST})



add_library(mengsen_base STATIC ${SRC_LIST})