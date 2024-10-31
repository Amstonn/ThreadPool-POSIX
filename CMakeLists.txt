cmake_minimum_required(VERSION 3.20)
project(threadpool)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE_DIR)
add_executable(threadpool ${SOURCE_DIR}) 