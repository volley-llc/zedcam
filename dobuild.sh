#!/bin/bash
mkdir -p build
pushd build
# If needed to specify cuda directory if more than one installed use:
# -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.6\
cmake -v -g ninja -DWITH_TEST=ON\
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON\
    -S .. -B .
make -j 6
popd
