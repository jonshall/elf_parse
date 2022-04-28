#!/bin/bash

# create bin dir if it doesn't exist
if [[ -d bin/ ]]; then
    echo "Bin Dir Exists; Skipping"
else
    mkdir ./bin/
fi

# create build dir if it doesn't exist
if [[ -d build/ ]]; then
    echo "Build Dir Exists; Skipping"
else
    mkdir ./build/
fi

rm -rf ./build/*
cd ./build/
cmake ..; make;
