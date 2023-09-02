#[[
# Author : KeeneChen
# Date   : 2022.10.10-19:00:39
#]]

cmake_minimum_required(VERSION 3.10.0)

project(
  APUE_Note
  VERSION 1.0.2
  HOMEPAGE_URL "https://github.com/Keene-Chen/APUE_Note"
  DESCRIPTION "APUE 阅读笔记"
  LANGUAGES C CXX)

# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包括colors.cmake模块,调用其中定义的宏
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)
define_colors()

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
  if("${CMAKE_BINARY_DIR}" MATCHES "build_.*(D|d)ebug$")
    message(STATUS "${Red}>>> Build Debug Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "Debug")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(R|r)elWithDebInfo$")
    message(STATUS "${Green}>>> Build RelWithDebInfo Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(M|m)inSizeRel$")
    message(STATUS "${Blue}>>> Build MinSizeRel Type>>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "MinSizeRel")
  else()
    message(STATUS "${BoldYellow}>>> Build Release Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

# 导出编译命令配置,供clangd使用
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 开启测试
enable_testing()

# 自定义生成可执行文件函数
function(add_exec ${SOURCE_FILES})
  # 循环遍历源文件列表
  foreach(FILE ${SOURCE_FILES})
    # 获取文件名（不包含扩展名）
    get_filename_component(FILENAME ${FILE} NAME_WE)
    # 添加二进制文件规则，命名方式为数字加文件名
    add_executable(${FILENAME} ${FILE})
  endforeach()
endfunction()

# 添加子目录
add_subdirectory(src/07_进程环境)
add_subdirectory(src/08_进程控制)
add_subdirectory(src/10_信号)
add_subdirectory(src/11_线程)
add_subdirectory(src/13_守护进程)
add_subdirectory(src/14_高级IO)
add_subdirectory(src/15_进程间通信)
