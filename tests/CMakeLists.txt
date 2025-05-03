cmake_minimum_required(VERSION 3.10)
project(AVLTreeTests)

set(CMAKE_CXX_STANDARD 17)

# Найти Google Test
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
# Добавить исполняемый файл
add_executable(avl_tree_tests tests.cpp)

# Линковка с Google Test
target_link_libraries(avl_tree_tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)