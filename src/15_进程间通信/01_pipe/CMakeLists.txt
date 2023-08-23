#[[
# Author : KeeneChen
# Date   : 2022.10.27-16:24:54
#]]

project(01_pipe DESCRIPTION "进程间通信-管道")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/15_IPC/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(01_pipe.c)
add_exec(02_mkfifo.c)
add_exec(03_mkfifo_read.c)
add_exec(04_mkfifo_write.c)
