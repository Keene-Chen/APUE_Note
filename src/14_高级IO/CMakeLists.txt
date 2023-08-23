#[[
# Author : KeeneChen
# Date   : 2022.10.31-22:10:23
#]]

project(14_advance_io DESCRIPTION "高级IO")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 添加编译选项
add_compile_options(-std=gnu11 -pthread)

# 定义源文件列表
set(SOURCE_FILES
    01_relay.c
    02_select_timer.c
    03_relay_select.c
    04_relay_poll.c
    05_relay_epoll.c
    06_mmap.c
    07_shared_memory.c
    08_lockf.c
    09_readv_writev.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
