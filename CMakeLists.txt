cmake_minimum_required(VERSION 3.10)
project(MnistClassifier)

# Point to the OpenCV build root directory, not the lib folder
set(OpenCV_DIR "C:/opencv/build" CACHE PATH "OpenCV dir")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(MnistClassifier main.cpp input.cpp)

target_link_libraries(MnistClassifier ${OpenCV_LIBS})