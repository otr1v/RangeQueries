add_subdirectory(gtest)
add_subdirectory(io_tests)

if (BENCHMARK)
    add_subdirectory(benchmark)
endif()