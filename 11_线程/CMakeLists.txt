cmake_minimum_required(VERSION 3.0.0)

project(multi_thread VERSION 1.0.0)

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 添加编译选项
add_compile_options(-std=gnu11 -pthread)

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PUBLIC pthread)
endfunction()

# 设置cmake构建时查看构建参数 set(CMAKE_VERBOSE_MAKEFILE ON)

# 1
add_test_file(01_pthread_create 01_pthread_create.c)
# 2
add_test_file(02_pthread_cleanup_push 02_pthread_cleanup_push.c)
# 3
add_test_file(03_demo_prime 03_demo_prime.c)
# 4
add_test_file(03_demo_prime_refactor 03_demo_prime_refactor.c)
# 5
add_test_file(04_demo_competition_issues 04_demo_competition_issues.c)
# 6
add_test_file(04_pthread_mutex_xxx 04_pthread_mutex_xxx.c)
# 7
add_test_file(04_demo_abcd 04_demo_abcd.c)
# 8
add_test_file(04_demo_prime_pool 04_demo_prime_pool.c)
