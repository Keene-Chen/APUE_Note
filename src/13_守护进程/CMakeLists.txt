#[[
# Author : KeeneChen
# Date   : 2022.10.13-19:01:56
#]]

project(13_daemon_process DESCRIPTION "守护进程")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES 01_daemon.c 02_daemon_refactor.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
