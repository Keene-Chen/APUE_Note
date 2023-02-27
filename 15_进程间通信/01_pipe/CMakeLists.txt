project(
  01_pipe
  VERSION 1.0.0
  DESCRIPTION "进程间通信-管道")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/lib/15_IPC/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/lib/15_IPC/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_executable(01_pipe 01_pipe.c)
add_executable(02_mkfifo 02_mkfifo.c)
add_executable(03_mkfifo_read 03_mkfifo_read.c)
add_executable(04_mkfifo_write 04_mkfifo_write.c)
