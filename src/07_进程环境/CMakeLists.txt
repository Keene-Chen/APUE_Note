#[[
# Author : KeeneChen
# Date   : 2022.10.10-18:33:49
#]]

project(07_process_environment DESCRIPTION "进程环境")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 自定义函数添加可执行文件
add_exec(01_atexit.c)
add_exec(02_environ.c)
add_exec(03_mydate.c)
add_exec(04_dlopen.c)
add_exec(05_setjmp.c)
