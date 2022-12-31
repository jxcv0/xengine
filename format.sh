#!/bin/bash
format_cmd="clang-format -i -style=Google"
files=($(find . -name "*.c" -o -name "*.h" | grep -v -E "lib/|build/"))

for file in ${files[@]}; do
    echo "formatting file: $file"
    $format_cmd $file
done
