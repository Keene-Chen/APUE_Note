project(
  03_sem
  VERSION 1.0.0
  DESCRIPTION "进程间通信-信号量集")

# 设置可执行文件、库生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_test_file(sem sem.c)
