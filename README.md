# Implemetation of AVL tree
Binary search and dynamic-balanced tree
## Made for task named "Range queries"
### Understanding the task:
There are two types of commands: 
- Key <code> k \<number> </code> that you insert into the tree
- Request <code> q \<bound1> \<bound2> </code> counts how many numbers are in [bound1, bound2]

**Important** to mention, that \<number> is a template, so the key can be even std::string. 

Example of input: <code> q 9 50 q 5 27 k 1 q -5 15 k 30 </code>
Output will be: <code> 0 0 1 </code>

## How to build
Build to use:
```
cmake -B build 
cmake --build build 
./build/range_queries
 ```
Build to test with GoogleTest:
```
cd tests/gtest
cmake -B build 
cmake --build build 
./build/avl_tree_tests
 ```

 Run script with 11 input/output tests: (**Path dependency**):
 ```
 cd tests/io_tests/
 chmod +x launch.sh 
 ./launch.sh
 ```