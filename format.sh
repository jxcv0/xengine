#!/bin/bash
find assetmgr -iname *.h -o -iname *.c | xargs clang-format -i -style=file
