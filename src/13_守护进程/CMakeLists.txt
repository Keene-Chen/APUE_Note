#[[
# Author : KeeneChen
# Date   : 2022.10.13-19:01:56
#]]

project(13_daemon_process DESCRIPTION "守护进程")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(01_daemon.c)
add_exec(02_daemon_refactor.c)
