#[[
# Author : KeeneChen
# Date   : 2022.10.19-16:01:38
#]]

project(11_multi_thread DESCRIPTION "多线程")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 添加编译选项
add_compile_options(-pthread)

# 设置cmake构建时查看构建参数 set(CMAKE_VERBOSE_MAKEFILE ON)

# 定义源文件列表
set(SOURCE_FILES
    01_pthread_create.c
    02_pthread_cleanup_push.c
    03_demo_prime.c
    04_demo_prime_refactor.c
    05_demo_competition_issues.c
    06_pthread_mutex_xxx.c
    07_demo_abcd.c
    08_demo_prime_pool.c
    09_pthread_cond_xxx.c
    10_demo_abcd_cond.c)

# 自定义函数生成可执行文件
add_exec(${SOURCE_FILES})
