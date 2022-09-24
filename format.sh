#!/bin/bash
FORMAT_CMD="clang-format-14 -i -style=Google"
${FORMAT_CMD} include/* src/*
pushd test > /dev/null
${FORMAT_CMD} *.cpp 
popd > /dev/null
