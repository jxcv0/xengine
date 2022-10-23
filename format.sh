#!/bin/bash
FORMAT_CMD="clang-format -i -style=Google"
${FORMAT_CMD} include/* src/*
pushd threadpool
${FORMAT_CMD} *.h *.cpp
popd
pushd memory
${FORMAT_CMD} *.h *.cpp
popd
pushd resource
${FORMAT_CMD} *.h *.cpp
popd
pushd ecs
${FORMAT_CMD} *.h *.cpp
popd
pushd test
${FORMAT_CMD} *.cpp 
popd
