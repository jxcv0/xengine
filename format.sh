#!/bin/bash
find assets -iname *.h -o -iname *.c | xargs clang-format -i -style=file
