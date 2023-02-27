project(
  10_signal
  VERSION 1.0.0
  DESCRIPTION "信号")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

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
