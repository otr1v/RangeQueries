# Implementation of AVL tree

Binary search and dynamic-balanced tree

## Made for task named "Range queries"

### Understanding the task:
There are two types of commands:
- `k <number>`: Inserts a key into the tree.
- `q <bound1> <bound2>`: Counts how many numbers are in the range `[bound1, bound2]`.

**Note:** The key type is templated, so it can be `int`, `std::string`, etc.

Example of input: <code> q 9 50 q 5 27 k 1 q -5 15 k 30 </code>
Output will be: <code> 0 0 1 </code>

## How to Build and Run

### Prerequisites
*   A C++23 compliant compiler (e.g., GCC 13+, Clang 16+).
*   CMake (version 3.29 or newer).
*   `glog` library
*   `gtest` library (for tests)
*   `clang-format` (for code formatting)

This project uses CMake Presets to manage build configurations. This simplifies the process and makes it consistent for all developers.

To see all available presets, run:
```bash
cmake --list-presets
```

### 1. Configuration

Choose configuration:

**For Release-build (optimized):**
```bash
cmake --preset release
cmake --build --preset release
./build/release/range_queries
```

**For Debug-build with Address и Undefined Behavior sanitiziers:**
```bash
cmake --preset default
cmake --build build/debug/
./build/debug/range_queries
```

**For Fuzzing:**
```bash
cmake --preset fuzz
cmake --build --preset fuzz
./build/fuzz/tests/fuzz/avl_tree_fuzzer {-runs=100000}
```


### 2. Running Tests

**Unit-тесты (Google Test):**
```bash
./build/tests/gtest/gtests
 ```

TARGET_NAME={debug, fuzz}
 Run ctest with 11 input/output tests (for now):
 ```
ctest --output-on-failure --test-dir build/${TARGET_NAME}
 ```


### 3. Code Formatting

Formatting using clang-format (not added to presets yet):
```bash
cmake --build build --target format
```

