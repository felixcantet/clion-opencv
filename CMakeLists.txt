cmake_minimum_required(VERSION 3.19)
project(ComputerVision)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)
add_executable(ComputerVision main.cpp HandDetection.cpp HandDetection.h InputHandler.cpp InputHandler.h)
include_directories(OpenCV_INCLUDE_DIRS)
#set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)
target_link_libraries(ComputerVision ${OpenCV_LIBS})
