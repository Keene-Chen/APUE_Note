#[[
# Author  : KeeneChen
# DateTime: 2022.10.24-8:50:33
#]]

cmake_minimum_required(VERSION 3.0.0)

project(
  mytbf
  VERSION 0.1.0
  DESCRIPTION "信号量")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# sem_test
add_library(mysem SHARED mysem.c)
add_executable(mysem_test mysem_test.c)
target_compile_options(mysem_test PRIVATE -std=gnu11 -pthread)
target_link_libraries(mysem_test PRIVATE mysem pthread)
