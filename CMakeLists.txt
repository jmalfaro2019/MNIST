cmake_minimum_required(VERSION 3.10)
project(MnistClassifier)

# Point to the OpenCV build root directory, not the lib folder
set(OpenCV_DIR "C:/opencv/build" CACHE PATH "OpenCV dir")

# Point to ONNX Runtime
set(ONNXRUNTIME_DIR "${CMAKE_SOURCE_DIR}/model/onnxruntime-win-x64-1.23.0")

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${ONNXRUNTIME_DIR}/include)

add_executable(MnistClassifier main.cpp input.cpp clasifier.cpp)

# Link OpenCV
target_link_libraries(MnistClassifier ${OpenCV_LIBS})

# Link ONNX Runtime
target_link_libraries(MnistClassifier ${ONNXRUNTIME_DIR}/lib/onnxruntime.lib)

# Copy DLLs to build directory after build
add_custom_command(TARGET MnistClassifier POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ONNXRUNTIME_DIR}/lib/onnxruntime.dll"
        "$<TARGET_FILE_DIR:MnistClassifier>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ONNXRUNTIME_DIR}/lib/onnxruntime_providers_shared.dll"
        "$<TARGET_FILE_DIR:MnistClassifier>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/model/mnist_cnn.onnx"
        "$<TARGET_FILE_DIR:MnistClassifier>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/model/mnist_cnn.onnx.data"
        "$<TARGET_FILE_DIR:MnistClassifier>"
)