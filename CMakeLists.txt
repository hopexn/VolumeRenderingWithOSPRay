cmake_minimum_required(VERSION 3.7)
project(OSPRayProject)

set(CMAKE_CXX_STANDARD 11)

#编译器设置为GCC 6.0
#set(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
#set(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

#解决无法找到ospray.h的问题
include_directories("/opt/local/include")
link_directories("/opt/local/lib")

#添加OSPRay依赖
find_package(OSPRay REQUIRED)

#QT
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
set(QT_USE_QTOPENGL TRUE)
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
set(QT_USE_QTOPENGL TRUE)

#项目源代码
set(HEAD_FILES RenderWidget.h Camera.h TF1D.h Volume.h util/Vector.h)

set(SOURCE_FILES main.cpp TF1D.cpp Volume.cpp Camera.cpp RenderWidget.cpp MainWIndow.cpp MainWIndow.h)

add_executable(OSPRayProject ${SOURCE_FILES} ${HEAD_FILES})

target_link_libraries(OSPRayProject ${OSPRAY_LIBRARIES}  ${Embree_libraries} Qt5::Widgets Qt5::OpenGL ${OPENGL_gl_LIBRARY})
