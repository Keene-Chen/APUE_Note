cmake_minimum_required(VERSION 3.10.0)

project(
  process_control
  VERSION 1.0.0
  DESCRIPTION "进程控制")

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
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 导出编译命令配置,供clangd使用
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  # target_link_libraries(${TEST_NAME} PUBLIC pthread)
endfunction()

add_test_file(01_fork 01_fork.c)
add_test_file(02_wait 02_wait.c)
add_test_file(03_waitpid 03_waitpid.c)
add_test_file(04_execl 04_execl.c)
add_test_file(05_few 05_few.c)
add_test_file(06_mysh 06_mysh.c)
add_test_file(07_mysu 07_mysu.c)
add_test_file(08_times 08_times.c)
