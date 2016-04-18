cmake_minimum_required(VERSION 2.8)

project(ImageProcessor)

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE ImageProcessor_SOURCES "*.cpp")
file(GLOB_RECURSE ImageProcessor_HEADERS "*.h")

set (ImageProcessor_INCLUDE_DIRS "")
foreach (_headerFile ${ImageProcessor_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND ImageProcessor_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES ImageProcessor_INCLUDE_DIRS)

include_directories(${ImageProcessor_INCLUDE_DIRS})
add_executable (ImageProcessor ${ImageProcessor_SOURCES})

target_link_libraries(ImageProcessor ${OpenCV_LIBS})
