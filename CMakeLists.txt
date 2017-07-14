cmake_minimum_required(VERSION 3.7)
project(OSPRayProject)

set(CMAKE_CXX_STANDARD 11)

#编译器设置为GCC 6.0
set(CMAKE_C_COMPILER "/usr/local/bin/gcc-6")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-6")

#解决无法找到ospray.h的问题
include_directories("/opt/local/include/")

#添加OSPRay依赖
find_package(OSPRay REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(OSPRayProject ${SOURCE_FILES})