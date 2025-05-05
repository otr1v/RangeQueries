#!/usr/bin/env bash

EXECUTABLE=$1
INPUT_FILE=$2
EXPECTED_OUTPUT_FILE=$3


if [ ! -x "$EXECUTABLE" ]; then
  echo "Error: Executable '$EXECUTABLE' not found or not executable." >&2
  exit 127 # Стандартный код ошибки "command not found"
fi

if [ ! -f "$INPUT_FILE" ]; then
  echo "Error: Input file '$INPUT_FILE' not found." >&2
  exit 1
fi

if [ ! -f "$EXPECTED_OUTPUT_FILE" ]; then
  echo "Error: Expected output file '$EXPECTED_OUTPUT_FILE' not found." >&2
  exit 1
fi


TEMP_OUTPUT_FILE=$(mktemp --suffix=.test_output)

trap "rm -f $TEMP_OUTPUT_FILE" EXIT


"$EXECUTABLE" < "$INPUT_FILE" > "$TEMP_OUTPUT_FILE"
EXEC_RET_CODE=$?


if [ $EXEC_RET_CODE -ne 0 ]; then
  echo "Error: Executable '$EXECUTABLE' exited with non-zero status ($EXEC_RET_CODE)." >&2
  exit $EXEC_RET_CODE
fi


diff "$TEMP_OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE"
DIFF_RET_CODE=$?

exit $DIFF_RET_CODE