project(
  04_shm
  VERSION 1.0.1
  DESCRIPTION "进程间通信-共享内存")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_exec(shm.c)
