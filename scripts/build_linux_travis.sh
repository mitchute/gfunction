#!/bin/bash -e

# build the tool
mkdir build
cd build
cmake ..
cmake --build ..
./gfunction_tests

# prep for packaging
mkdir tmp_build
cp build/gfunction tmp_build/
cp -r examples tmp_build/

# create final package
mkdir release
tar -zcvf release/gfunction_Linux.tar.gz -C tmp_build examples