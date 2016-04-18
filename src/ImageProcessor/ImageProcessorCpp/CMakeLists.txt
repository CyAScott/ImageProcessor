cmake_minimum_required(VERSION 2.8)

project(ImageProcessor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

add_executable(ImageProcessor masterHeaders.cpp arguments.cpp rawImage.cpp grayscaleFilter.cpp medianFilter.cpp thresholdFilter.cpp scaleFilter.cpp grayHistogram.cpp grayHistogramEqualization.cpp histogramStretch.cpp histogramEqualization.cpp hsiHistogramEqualization.cpp sobelEdgeDetection.cpp openCvCannyEdgeDetection.cpp openCvGrayHistogramEqualization.cpp openCvHistogramEqualization.cpp openCvSobelEdgeDetection.cpp openCvYCrCbHistogramEqualization.cpp filterPipe.cpp program.cpp)

target_link_libraries(ImageProcessor ${OpenCV_LIBS})
