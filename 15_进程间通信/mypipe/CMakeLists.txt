cmake_minimum_required(VERSION 3.0.0)

project(
  mypipe
  VERSION 1.0.0
  DESCRIPTION "管道实现")

# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 导出编译命令配置,供clangd使用
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_library(mypipe SHARED mypipe.c)

add_executable(mypipe_test mypipe_test.c)
target_compile_options(mypipe_test PRIVATE -pthread)
target_link_libraries(mypipe_test PUBLIC mypipe pthread)
