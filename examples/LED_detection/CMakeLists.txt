cmake_minimum_required(VERSION 3.5)

project(LEDdetection)

find_package(OpenCV REQUIRED)

add_executable(LEDdetection LED_detection.cpp)

target_include_directories(LEDdetection PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( LEDdetection ${OpenCV_LIBS} )
target_link_libraries( LEDdetection Kilobot_detection_library )