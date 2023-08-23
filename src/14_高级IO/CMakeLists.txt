#[[
# Author : KeeneChen
# Date   : 2022.10.31-22:10:23
#]]

project(14_advance_io DESCRIPTION "高级IO")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 添加编译选项
add_compile_options(-std=gnu11 -pthread)

# 自定义函数添加可执行文件
add_exec(01_relay.c)
add_exec(02_select_timer.c)
add_exec(03_relay_select.c)
add_exec(04_relay_poll.c)
add_exec(05_relay_epoll.c)
add_exec(06_mmap.c)
add_exec(07_shared_memory.c)
add_exec(08_lockf.c)
add_exec(09_readv_writev.c)
