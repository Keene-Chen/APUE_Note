#[[
# Author : KeeneChen
# Date   : 2022.10.30-21:13:34
#]]

project(02_msg DESCRIPTION "进程间通信-消息队列")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES rcver.c snder.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
