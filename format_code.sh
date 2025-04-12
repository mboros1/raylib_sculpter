#!/bin/bash

# Format all C source and header files in src/ and include/ directories
find src include -name "*.c" -o -name "*.h" | xargs clang-format -i

echo "Code formatting complete!" 