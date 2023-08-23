#[[
# Author : KeeneChen
# Date   : 2022.10.14-13:50:31
#]]

project(10_signal DESCRIPTION "信号")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 定义源文件列表
set(SOURCE_FILES
    01_signal.c
    02_alarm.c
    03_5sec_volatile.c
    04_5sec_time.c
    05_5sec_signal.c
    06_mycat.c
    07_slowcat.c
    08_slowcat2.c
    09_slowcat_setitimer.c
    10_sigset.c
    11_sigsuspend.c
    12_daemon_sigaction.c
    13_sigsuspend_rt.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
