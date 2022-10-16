#!/bin/bash
FORMAT_CMD="clang-format -i -style=Google"
${FORMAT_CMD} include/* src/*
pushd test > /dev/null
${FORMAT_CMD} *.cpp 
popd > /dev/null
