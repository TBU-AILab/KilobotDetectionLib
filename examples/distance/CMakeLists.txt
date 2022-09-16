cmake_minimum_required(VERSION 3.5)

project(DistanceExample)

find_package(OpenCV REQUIRED)

add_executable(DistanceExample distance_example.cpp)

target_include_directories(DistanceExample PRIVATE ${YOUR_DIRECTORY} ../../include  ${OpenCV_INCLUDE_DIRS})
target_link_libraries( DistanceExample ${OpenCV_LIBS} )
target_link_libraries( DistanceExample kilolib )