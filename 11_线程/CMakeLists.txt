project(
  11_multi_thread
  VERSION 1.0.0
  DESCRIPTION "多线程")

# 设置可执行文件、库生成位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/lib/${PROJECT_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  # target_link_libraries(${TEST_NAME} PUBLIC pthread)
endfunction()

# 添加编译选项
add_compile_options(-pthread)

# 设置cmake构建时查看构建参数 set(CMAKE_VERBOSE_MAKEFILE ON)

add_test_file(01_pthread_create 01_pthread_create.c)
add_test_file(02_pthread_cleanup_push 02_pthread_cleanup_push.c)
add_test_file(03_demo_prime 03_demo_prime.c)
add_test_file(04_demo_prime_refactor 04_demo_prime_refactor.c)
add_test_file(05_demo_competition_issues 05_demo_competition_issues.c)
add_test_file(06_pthread_mutex_xxx 06_pthread_mutex_xxx.c)
add_test_file(07_demo_abcd 07_demo_abcd.c)
add_test_file(08_demo_prime_pool 08_demo_prime_pool.c)
add_test_file(09_pthread_cond_xxx 09_pthread_cond_xxx.c)
add_test_file(10_demo_abcd_cond 10_demo_abcd_cond.c)
