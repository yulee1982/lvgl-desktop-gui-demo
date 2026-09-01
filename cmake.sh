#!/bin/sh

rm -rf build
mkdir -p build
cd build/
cmake -B build -GNinja -DCMAKE_TOOLCHAIN_FILE=./user_cross_compile_setup.cmake ..
cmake --build build
