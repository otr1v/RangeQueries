cmake_minimum_required(VERSION 3.29)
project(range_queries)

find_package(glog REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glog::glog)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

enable_testing()
add_subdirectory(tests)