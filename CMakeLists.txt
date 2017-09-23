CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(DisplayImg)
find_package(OpenCV REQUIRED)

include_directories(include/)

add_executable(DisplayImg src/input_conv_main.cpp src/ImageSC.cpp src/base.cpp)
target_link_libraries(DisplayImg ${OpenCV_LIBS})
