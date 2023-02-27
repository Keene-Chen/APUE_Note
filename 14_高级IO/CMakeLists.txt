project(
  14_advance_io
  VERSION 1.0.0
  DESCRIPTION "高级IO")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

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
