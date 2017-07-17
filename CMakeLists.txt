cmake_minimum_required(VERSION 3.7)
project(OSPRayProject)

set(CMAKE_CXX_STANDARD 11)

#编译器设置为GCC 6.0, 支持OpenMP
set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wall")

#添加OSPRay依赖
find_package(OSPRay REQUIRED)
include_directories("/opt/local/include")
link_directories("/opt/local/lib")

#QT
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

#QT开启OpenGL
set(QT_USE_QTOPENGL TRUE)
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

#OpenCV
find_package(OpenCV REQUIRED)

#项目源代码
set(SOURCE_FILES main.cpp Camera.cpp TF1D.cpp Volume.cpp common.h Camera.cpp)

#可执行文件
add_executable(OSPRayProject ${SOURCE_FILES})

#链接
target_link_libraries(OSPRayProject ${OSPRAY_LIBRARIES} ${OpenCV_LIBS} Qt5::Widgets)
