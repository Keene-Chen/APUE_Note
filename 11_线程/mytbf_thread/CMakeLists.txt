#[[
# Author  : KeeneChen
# DateTime: 2022.10.21-9:50:33
#]]

cmake_minimum_required(VERSION 3.0.0)

project(
  mytbf
  VERSION 0.1.2
  DESCRIPTION "令牌桶封装(多线程)实现")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# mytbf_signal
add_library(mytbf_signal SHARED mytbf_signal.c)
add_executable(mytbf_test1 mytbf_test.c)
target_link_libraries(mytbf_test1 PRIVATE mytbf_signal)

# mytbf_sigaction
add_library(mytbf_sigaction SHARED mytbf_sigaction.c)
add_executable(mytbf_test2 mytbf_test.c)
target_link_libraries(mytbf_test2 PRIVATE mytbf_sigaction)

# mytbf_thread
add_library(mytbf_thread SHARED mytbf_thread.c)
add_executable(mytbf_test3 mytbf_test.c)
target_compile_options(mytbf_test3 PRIVATE -std=gnu11 -pthread)
target_link_libraries(mytbf_test3 PRIVATE mytbf_thread pthread)

# mytbf_thread_cond
add_library(mytbf_thread_cond SHARED mytbf_thread_cond.c)
add_executable(mytbf_test4 mytbf_test.c)
target_compile_options(mytbf_test4 PRIVATE -std=gnu11 -pthread)
target_link_libraries(mytbf_test4 PRIVATE mytbf_thread_cond pthread)