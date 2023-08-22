project(
  10_signal
  VERSION 1.0.1
  DESCRIPTION "信号")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_exec(01_signal.c)
add_exec(02_alarm.c)
add_exec(03_5sec_volatile.c)
add_exec(04_5sec_time.c)
add_exec(05_5sec_signal.c)
add_exec(06_mycat.c)
add_exec(07_slowcat.c)
add_exec(08_slowcat2.c)
add_exec(09_slowcat_setitimer.c)
add_exec(10_sigset.c)
add_exec(11_sigsuspend.c)
add_exec(12_daemon_sigaction.c)
add_exec(13_sigsuspend_rt.c)
