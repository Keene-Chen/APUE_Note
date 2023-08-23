#[[
# Author : KeeneChen
# Date   : 2022.10.27-16:24:54
#]]

project(01_pipe DESCRIPTION "进程间通信-管道")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES 01_pipe.c 02_mkfifo.c 03_mkfifo_read.c 04_mkfifo_write.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
