project(
  07_process_environment
  VERSION 1.0.0
  DESCRIPTION "进程环境")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  # target_link_libraries(${TEST_NAME} PUBLIC pthread)
endfunction()

add_test_file(01_atexit 01_atexit.c)
add_test_file(02_environ 02_environ.c)
add_test_file(03_mydate 03_mydate.c)
add_test_file(04_dlopen 04_dlopen.c)
add_test_file(05_setjmp 05_setjmp.c)
