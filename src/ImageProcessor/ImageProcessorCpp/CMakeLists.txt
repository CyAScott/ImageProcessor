cmake_minimum_required(VERSION 2.8)

project(ImageProcessor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SRC_FILES *.cpp)
add_executable(ImageProcessor ${SRC_FILES})

target_link_libraries(ImageProcessor ${OpenCV_LIBS})
