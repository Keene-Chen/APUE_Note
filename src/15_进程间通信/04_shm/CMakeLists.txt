#[[
# Author : KeeneChen
# Date   : 2022.10.31-19:14:29
#]]

project(04_shm DESCRIPTION "进程间通信-共享内存")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES shm.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
