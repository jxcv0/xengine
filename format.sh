#!/bin/bash
style="Google"
find assets -iname *.h -o -iname *.c | xargs clang-format -i -style=${style}
find ecs -iname *.h -o -iname *.c | xargs clang-format -i  -style=${style}
find ecs2 -iname *.h -o -iname *.cpp | xargs clang-format -i  -style=${style}
