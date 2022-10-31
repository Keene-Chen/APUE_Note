cmake_minimum_required(VERSION 3.0.0)

project(
  advance_io
  VERSION 1.0.0
  DESCRIPTION "高级IO")

# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
  if("${CMAKE_BINARY_DIR}" MATCHES "build_.*(D|d)ebug$")
    message(STATUS ">>> Build Debug Type >>>")
    set(CMAKE_BUILD_TYPE "Debug")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(R|r)elWithDebInfo$")
    message(STATUS ">>> Build RelWithDebInfo Type >>>")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(M|m)inSizeRel$")
    message(STATUS ">>> Build MinSizeRel Type>>>")
    set(CMAKE_BUILD_TYPE "MinSizeRel")
  else()
    message(STATUS ">>> Build Release Type >>>")
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 添加编译选项
add_compile_options(-std=gnu11 -pthread)

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PUBLIC pthread)
endfunction()

add_test_file(01_relay 01_relay.c)
add_test_file(02_select_timer 02_select_timer.c)
add_test_file(03_relay_select 03_relay_select.c)
add_test_file(04_relay_poll 04_relay_poll.c)
add_test_file(05_relay_epoll 05_relay_epoll.c)
add_test_file(06_mmap 06_mmap.c)
add_test_file(07_shared_memory 07_shared_memory.c)
add_test_file(08_lockf 08_lockf.c)
add_test_file(09_readv_writev 09_readv_writev.c)
