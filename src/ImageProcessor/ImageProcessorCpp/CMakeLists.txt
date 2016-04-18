cmake_minimum_required(VERSION 3.0)

project(ImageProcessor)

add_compile_options(-std=c++11)

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE SRC_FILES *.cpp)
add_executable(ImageProcessor ${SRC_FILES})

target_link_libraries(ImageProcessor ${OpenCV_LIBS})
