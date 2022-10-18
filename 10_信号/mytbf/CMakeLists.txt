#[[
# Author  : KeeneChen
# DateTime: 2022.10.17-17:30:43
#]]

cmake_minimum_required(VERSION 3.0.0)

project(
  mytbf
  VERSION 0.1.0
  DESCRIPTION "令牌桶封装实现")

add_library(mytbf SHARED mytbf.c)

add_executable(mytbf_test mytbf_test.c)
target_link_libraries(mytbf_test mytbf)
