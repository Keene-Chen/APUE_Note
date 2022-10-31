project(
  01_pipe
  VERSION 1.0.0
  DESCRIPTION "进程间通信-管道")

# 设置可执行文件、库生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_test_file(01_pipe 01_pipe.c)
add_test_file(02_mkfifo 02_mkfifo.c)
add_test_file(03_mkfifo_read 03_mkfifo_read.c)
add_test_file(04_mkfifo_write 04_mkfifo_write.c)
