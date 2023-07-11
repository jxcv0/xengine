#!/bin/bash
find assets -iname *.h -o -iname *.c | xargs clang-format -i -style=file
find ecs -iname *.h -o -iname *.c | xargs clang-format -i -style=file
find ecs2 -iname *.h -o -iname *.cpp | xargs clang-format -i -style=file
