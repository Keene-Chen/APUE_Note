project(
  03_sem
  VERSION 1.0.1
  DESCRIPTION "进程间通信-信号量集")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_exec(sem.c)
