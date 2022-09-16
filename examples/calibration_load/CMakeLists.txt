cmake_minimum_required(VERSION 3.5)

project(CalibrationLoadingExample)

find_package(OpenCV REQUIRED)

add_executable(CalibrationLoadingExample camera_calibration_load_example.cpp)

target_include_directories(CalibrationLoadingExample PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( CalibrationLoadingExample ${OpenCV_LIBS} )
target_link_libraries( CalibrationLoadingExample kilolib )