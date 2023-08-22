project(
  02_msg
  VERSION 1.0.1
  DESCRIPTION "进程间通信-消息队列")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

add_exec(rcver.c)
add_exec(snder.c)
