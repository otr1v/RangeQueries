
set(IO_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(INPUT_DIR ${IO_TEST_DIR}/input_tests)
set(OUTPUT_DIR ${IO_TEST_DIR}/output_tests)
set(SINGLE_TEST_SCRIPT ${IO_TEST_DIR}/launch.sh)

file(GLOB INPUT_FILES RELATIVE ${INPUT_DIR} ${INPUT_DIR}/test_input*.txt)


if(NOT EXISTS ${SINGLE_TEST_SCRIPT})
    message(FATAL_ERROR "Single test script not found: ${SINGLE_TEST_SCRIPT}")
endif()


foreach(input_filename ${INPUT_FILES})
    string(REGEX MATCH "test_input([0-9]+)\\.txt" test_match_var ${input_filename})
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(WARNING "Could not parse test number from input file: ${input_filename}")
        continue() 
    endif()
    set(test_number ${CMAKE_MATCH_1})

    
    set(test_name "io_test_${test_number}")
    set(current_input_file "${INPUT_DIR}/${input_filename}")
    set(expected_output_file "${OUTPUT_DIR}/test_output${test_number}.txt")

   
    if(NOT EXISTS ${expected_output_file})
        message(WARNING "Missing expected output file for ${input_filename}: ${expected_output_file}. Skipping test ${test_name}.")
        continue()
    endif()

    
    add_test(
        NAME ${test_name}
       
        COMMAND bash ${SINGLE_TEST_SCRIPT}
            $<TARGET_FILE:${PROJECT_NAME}>   
            ${current_input_file}           
            ${expected_output_file}      
        
        
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

   
endforeach()