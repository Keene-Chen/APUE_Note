cmake_minimum_required(VERSION 3.10.0)

project(
  signal
  VERSION 1.0.0
  DESCRIPTION "信号")

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

add_test_file(01_signal 01_signal.c)
add_test_file(02_alarm 02_alarm.c)
add_test_file(03_5sec_volatile 03_5sec_volatile.c)
add_test_file(04_5sec_time 04_5sec_time.c)
add_test_file(05_5sec_signal 05_5sec_signal.c)
add_test_file(06_mycat 06_mycat.c)
add_test_file(07_slowcat 07_slowcat.c)
add_test_file(08_slowcat2 08_slowcat2.c)
add_test_file(09_slowcat_setitimer 09_slowcat_setitimer.c)
add_test_file(10_sigset 10_sigset.c)
add_test_file(11_sigsuspend 11_sigsuspend.c)
add_test_file(12_daemon_sigaction 12_daemon_sigaction.c)
add_test_file(13_sigsuspend_rt 13_sigsuspend_rt.c)
