#!/bin/bash
mkdir -p build
pushd build
cmake -v -g ninja -DWITH_TEST=ON  -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.6 -S .. -B . 
make -j 6
popd
