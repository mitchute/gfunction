#!/bin/bash -e

# build the tool
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 2
cd ..
./build/gfunction ./gfunction_tests

# prep for packaging
mkdir tmp_build
cp build/gfunction tmp_build/
cp -r examples tmp_build/

# create final package
mkdir release
tar -zcvf release/gfunction_Linux.tar.gz -C tmp_build examples