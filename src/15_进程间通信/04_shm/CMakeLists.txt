#[[
# Author : KeeneChen
# Date   : 2022.10.31-19:14:29
#]]

project(
  04_shm
  DESCRIPTION "进程间通信-共享内存")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(shm.c)
