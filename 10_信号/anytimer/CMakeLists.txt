#[[
# Author  : KeeneChen
# DateTime: 2022.10.17-17:30:43
#]]

cmake_minimum_required(VERSION 3.0.0)

project(
  anytimer
  VERSION 0.1.0
  DESCRIPTION "多任务计时器")

add_library(anytimer SHARED anytimer.c)

add_executable(anytimer_test anytimer_test.c)
target_link_libraries(anytimer_test anytimer)
