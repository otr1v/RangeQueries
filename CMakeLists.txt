cmake_minimum_required(VERSION 3.29) 
project(AVLTree)

find_package(glog REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(FUZZ "Build fuzzer targets" OFF)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_library(AVLTreeLogic INTERFACE)

target_include_directories(
    AVLTreeLogic INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(AVLTreeLogic INTERFACE glog::glog)

target_sources(AVLTreeLogic INTERFACE
    include/avl_tree.hpp
    include/tree_exceptions.hpp
)

add_executable(range_queries main.cpp)
target_link_libraries(range_queries PRIVATE glog::glog AVLTreeLogic)
target_compile_options(range_queries PRIVATE -Wall -Wextra -g)
list(APPEND ALL_FORMAT_TARGETS range_queries)

option(BUILD_TESTING "Build the tests for the project" ON) 
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests) 
endif()

find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    # Собираем список файлов для форматирования с помощью генераторных выражений.
    # Этот подход надежно получает абсолютные пути ко всем файлам.
    set(FILES_TO_FORMAT
        $<TARGET_PROPERTY:AVLTreeLogic,INTERFACE_SOURCES> # Заголовочные файлы
        $<TARGET_FILE:range_queries> # Исходники основного приложения
        $<$<BOOL:${BUILD_TESTING}>:$<TARGET_FILE:gtests>> # Исходники gtest (если включены)
        $<$<BOOL:${FUZZ}>:$<TARGET_FILE:avl_tree_fuzzer>> # Исходники фаззера (если включен)
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${FILES_TO_FORMAT}
        COMMAND_EXPAND_LISTS
        COMMENT "Formatting all C++ files with clang-format..."
    )
endif()
