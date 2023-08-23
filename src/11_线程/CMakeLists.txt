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

# 自定义函数添加可执行文件
add_exec(01_pthread_create.c)
add_exec(02_pthread_cleanup_push.c)
add_exec(03_demo_prime.c)
add_exec(04_demo_prime_refactor.c)
add_exec(05_demo_competition_issues.c)
add_exec(06_pthread_mutex_xxx.c)
add_exec(07_demo_abcd.c)
add_exec(08_demo_prime_pool.c)
add_exec(09_pthread_cond_xxx.c)
add_exec(10_demo_abcd_cond.c)
