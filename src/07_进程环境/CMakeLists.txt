project(
  07_process_environment
  VERSION 1.0.1
  DESCRIPTION "进程环境")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_exec(01_atexit.c)
add_exec(02_environ.c)
add_exec(03_mydate.c)
add_exec(04_dlopen.c)
add_exec(05_setjmp.c)
