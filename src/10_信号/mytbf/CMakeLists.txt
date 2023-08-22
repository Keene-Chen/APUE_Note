#[[
# Author  : KeeneChen
# DateTime: 2022.10.18-20:45:56
#]]

cmake_minimum_required(VERSION 3.0.0)

project(
  mytbf
  VERSION 0.1.1
  DESCRIPTION "令牌桶封装实现")

add_library(mytbf SHARED mytbf.c)
add_executable(mytbf_test1 mytbf_test.c)
target_link_libraries(mytbf_test1 mytbf)

add_library(mytbf_refactor SHARED mytbf_refactor.c)
add_executable(mytbf_test2 mytbf_test.c)
target_link_libraries(mytbf_test2 mytbf_refactor)
