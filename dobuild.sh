#!/bin/bash
mkdir -p build
pushd build
cmake -v -g ninja -DWITH_TEST=ON  -S .. -B . 
make -j 6
popd
