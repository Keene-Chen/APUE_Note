project(
  04_shm
  VERSION 1.0.0
  DESCRIPTION "进程间通信-共享内存")

# 设置可执行文件、库生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_test_file(shm shm.c)
