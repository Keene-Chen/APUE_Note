#[[
# Author : KeeneChen
# Date   : 2022.10.31-15:50:58
#]]

project(
03_sem
  DESCRIPTION "进程间通信-信号量集")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(sem.c)
