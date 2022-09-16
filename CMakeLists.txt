cmake_minimum_required(VERSION 3.5)
project(kilolib)

option(BUILD_EXAMPLES "Compile all examples" OFF)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)


find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(INC "${CMAKE_CURRENT_LIST_DIR}/include")
set(SRC "${CMAKE_CURRENT_LIST_DIR}/src")

file(GLOB LIB_SOURCES LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB LIB_HEADERS LIST_DIRECTORIES false CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE ${LIB_SOURCES} PUBLIC ${LIB_HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC} PRIVATE ${SRC})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

