project(
  01_pipe
  VERSION 1.0.1
  DESCRIPTION "进程间通信-管道")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_exec(01_pipe.c)
add_exec(02_mkfifo.c)
add_exec(03_mkfifo_read.c)
add_exec(04_mkfifo_write.c)
