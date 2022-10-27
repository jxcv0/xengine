#!/bin/bash
FORMAT_CMD="clang-format -i -style=Google"
find . -name "*.cpp" -o -name "*.h" | xargs $FORMAT_CMD
