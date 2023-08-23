#[[
# Author : KeeneChen
# Date   : 2022.10.30-21:13:34
#]]

project(02_msg DESCRIPTION "进程间通信-消息队列")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(rcver.c)
add_exec(snder.c)
