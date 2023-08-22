project(
  08_process_control
  VERSION 1.0.1
  DESCRIPTION "进程控制")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

add_exec(01_fork.c)
add_exec(02_wait.c)
add_exec(03_waitpid.c)
add_exec(04_execl.c)
add_exec(05_few.c)
add_exec(06_mysh.c)
add_exec(07_mysu.c)
add_exec(08_times.c)
