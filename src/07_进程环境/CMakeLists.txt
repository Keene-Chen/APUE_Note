#[[
# Author : KeeneChen
# Date   : 2022.10.10-18:33:49
#]]

project(07_process_environment DESCRIPTION "进程环境")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES 01_atexit.c 02_environ.c 03_mydate.c 04_dlopen.c 05_setjmp.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
