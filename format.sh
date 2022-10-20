#!/bin/bash
FORMAT_CMD="clang-format -i -style=Google"
${FORMAT_CMD} include/* src/*
pushd include/threadpool
${FORMAT_CMD} *.h *.cpp
popd
pushd test
${FORMAT_CMD} *.cpp 
popd
