cmake_minimum_required(VERSION 2.8.12)

project(decompose)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(decompose decompose.cpp)
target_link_libraries(decompose ${OpenCV_LIBS})
