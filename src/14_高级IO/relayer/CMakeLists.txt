cmake_minimum_required(VERSION 3.0.0)

project(
  relayer
  VERSION 1.0.0
  DESCRIPTION "数据中继引擎实现")

# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
message(STATUS ">>> C STANDARD:${CMAKE_C_STANDARD} CXX STANDARD:${CMAKE_CXX_STANDARD} >>>")

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
  if("${CMAKE_BINARY_DIR}" MATCHES ".*(D|d)ebug$")
    message(STATUS ">>> No build type selected, default to *** Debug *** >>>")
    set(CMAKE_BUILD_TYPE "Debug")
  else()
    message(STATUS ">>> No build type selected, default to *** Release *** >>>")
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 添加编译选项
add_compile_options(-std=gnu11 -pthread)

# 添加mimalloc库
find_library(mimalloc libmimalloc.so lib/mimalloc-2.0 REQUIRED)
add_library(relayer SHARED relayer.c)

# 生成可执行文件
add_executable(relayer_test relayer_test.c)
target_include_directories(relayer_test PUBLIC lib)
target_link_libraries(relayer_test PUBLIC relayer mimalloc pthread)
