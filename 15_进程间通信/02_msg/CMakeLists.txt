project(
  02_msg
  VERSION 1.0.0
  DESCRIPTION "进程间通信-消息队列")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/lib/15_IPC/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin/lib/15_IPC/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_executable(rcver rcver.c)
add_executable(snder snder.c)
