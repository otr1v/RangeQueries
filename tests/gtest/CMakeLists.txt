cmake_minimum_required(VERSION 3.10)
project(gtests)

set(CMAKE_CXX_STANDARD 17)


find_package(GTest REQUIRED)
find_package(glog REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


add_executable(gtests tests.cpp)
target_link_libraries(gtests PRIVATE AVLTreeLogic ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread glog::glog)
