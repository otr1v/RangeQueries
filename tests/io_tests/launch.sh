#!/bin/bash

EXECUTABLE="../../build/range_queries"
INPUT_DIR="input_tests"
OUTPUT_DIR="output_tests"
NUM_TESTS=11
PASSED_COUNT=0
FAILED_COUNT=0

if [ ! -x "$EXECUTABLE" ]; then
  echo "Error: Executable '$EXECUTABLE' not found or not executable."
  exit 1
fi

if [ ! -d "$INPUT_DIR" ]; then
  echo "Error: Input directory '$INPUT_DIR' not found."
  exit 1
fi

if [ ! -d "$OUTPUT_DIR" ]; then
  echo "Error: Output directory '$OUTPUT_DIR' not found."
  exit 1
fi

echo "Starting tests for '$EXECUTABLE'..."
echo "--------------------------------------------------"

TEMP_OUTPUT_FILE=$(mktemp)
trap "rm -f $TEMP_OUTPUT_FILE" EXIT

for i in $(seq 1 $NUM_TESTS); do

  CURRENT_INPUT_FILENAME="test_input${i}.txt"

  
  CURRENT_INPUT_FILE="${INPUT_DIR}/${CURRENT_INPUT_FILENAME}"
  CURRENT_EXPECTED_OUTPUT_FILE="${OUTPUT_DIR}/test_output${i}.txt" 

  echo -n "Test $i (${CURRENT_INPUT_FILENAME} -> ${EXECUTABLE} -> test_output${i}.txt): "

  if [ ! -f "$CURRENT_INPUT_FILE" ]; then
    echo "FAILED (Missing input file: $CURRENT_INPUT_FILE)"
    FAILED_COUNT=$((FAILED_COUNT + 1))
    continue 
  fi
  if [ ! -f "$CURRENT_EXPECTED_OUTPUT_FILE" ]; then
    echo "FAILED (Missing expected output file: $CURRENT_EXPECTED_OUTPUT_FILE)"
    FAILED_COUNT=$((FAILED_COUNT + 1))
    continue 
  fi

  "$EXECUTABLE" < "$CURRENT_INPUT_FILE" > "$TEMP_OUTPUT_FILE"

  diff -q "$TEMP_OUTPUT_FILE" "$CURRENT_EXPECTED_OUTPUT_FILE" > /dev/null 2>&1

  if [ $? -eq 0 ]; then
    echo "PASSED"
    PASSED_COUNT=$((PASSED_COUNT + 1))
  else
    echo "FAILED"
    FAILED_COUNT=$((FAILED_COUNT + 1))
  fi

done

rm -f "$TEMP_OUTPUT_FILE"

echo "--------------------------------------------------"
echo "Testing finished."
echo "Results: ${PASSED_COUNT} passed, ${FAILED_COUNT} failed."
echo "--------------------------------------------------"

if [ "$FAILED_COUNT" -gt 0 ]; then
  exit 1
else
  exit 0
fi