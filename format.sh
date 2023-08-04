#!/bin/bash
style="LLVM"
find assets -iname *.h -o -iname *.c | xargs clang-format -i -style=${style}
find tables -iname *.hpp -o -iname *.cpp | xargs clang-format -i -style=${style}
