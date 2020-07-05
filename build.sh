#!/bin/bash

set -e
set -x

rm -rf build
mkdir build
pushd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
popd
