cmake_minimum_required(VERSION 3.5)

project(DetectorAndTrackerExample)

find_package(OpenCV REQUIRED)

add_executable(DetectorAndTrackerExample detector_and_tracker_example.cpp)

target_include_directories(DetectorAndTrackerExample PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( DetectorAndTrackerExample ${OpenCV_LIBS} )
target_link_libraries( DetectorAndTrackerExample kilolib )