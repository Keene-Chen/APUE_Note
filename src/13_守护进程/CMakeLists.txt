project(
  13_daemon_process
  VERSION 1.0.1
  DESCRIPTION "守护进程")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_exec(01_daemon.c)
add_exec(02_daemon_refactor.c)
