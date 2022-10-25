cmake_minimum_required(VERSION 3.5)

project(CalibrationStoreExample)

find_package(OpenCV REQUIRED)

add_executable(CalibrationStoreExample camera_calibration_store_example.cpp)

target_include_directories(CalibrationStoreExample PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( CalibrationStoreExample ${OpenCV_LIBS} )
target_link_libraries( CalibrationStoreExample kilolib )