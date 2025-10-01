cmake_minimum_required(VERSION 3.16)
project(fuzz_tests)

if (FUZZ)
    find_package(glog REQUIRED CONFIG)

    add_executable(avl_tree_fuzzer fuzz_target.cpp)
    target_compile_features(avl_tree_fuzzer PRIVATE cxx_std_17)
    target_link_libraries(avl_tree_fuzzer PRIVATE AVLTreeLogic glog::glog)

    target_compile_options(avl_tree_fuzzer PRIVATE -fsanitize=fuzzer)
    target_link_options(avl_tree_fuzzer PRIVATE -fsanitize=fuzzer)

    list(APPEND ALL_FORMAT_TARGETS avl_tree_fuzzer)
endif()