#!/bin/bash

for test in build/bin/test/*; do
    "./$test"
    echo $?
done