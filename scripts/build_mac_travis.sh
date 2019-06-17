#!/bin/bash -e

# build the tool
mkdir build
cd build
cmake ..
cmake --build .
./gfunction_tests

# prep for packaging
cd ..
mkdir tmp_build
mkdir tmp_build/examples
cp build/gfunction_runner tmp_build/
cp -r examples tmp_build/examples

# create final package
mkdir release
tar -zcvf release/gfunction_mac.tar.gz -C tmp_build examples gfunction_runner
