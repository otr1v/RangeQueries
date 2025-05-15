cmake_minimum_required(VERSION 3.29)
project(AVLTree)

find_package(glog REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(AVLTreeLogic INTERFACE)

target_include_directories(AVLTreeLogic INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(range_queries main.cpp)
target_link_libraries(range_queries PRIVATE glog::glog AVLTreeLogic)
target_compile_options(range_queries PRIVATE -Wall -Wextra)

option(BUILD_TESTING "Build the tests for the project" ON)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

