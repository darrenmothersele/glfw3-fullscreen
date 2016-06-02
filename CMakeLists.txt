cmake_minimum_required(VERSION 3.5)
project(fullscreen)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_library(cyril_window Window.cpp)

set(SOURCE_FILES main.cpp)
add_executable(fullscreen ${SOURCE_FILES})
target_link_libraries(fullscreen cyril_window ${OPENGL_LIBRARIES} ${GLFW_STATIC_LIBRARIES})
