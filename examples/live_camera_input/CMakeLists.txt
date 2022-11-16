cmake_minimum_required(VERSION 3.5)

project(LiveCameraInput)

find_package(OpenCV REQUIRED)

add_executable(LiveCameraInput live_camera_input.cpp)

target_include_directories(LiveCameraInput PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( LiveCameraInput ${OpenCV_LIBS} )
target_link_libraries( LiveCameraInput Kilobot_detection_library )