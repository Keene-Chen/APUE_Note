#[[
# Author : KeeneChen
# Date   : 2022.10.12-18:54:23
#]]

project(08_process_control DESCRIPTION "进程控制")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES
    01_fork.c
    02_wait.c
    03_waitpid.c
    04_execl.c
    05_few.c
    06_mysh.c
    07_mysu.c
    08_times.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
