#!/usr/bin/env bash

# build the tool
mkdir build
cd build
cmake .. -G "Visual Studio 15 Win64"
cmake --build . --config Release
./Release/gfunction_tests

# prep for packaging
cd ..
mkdir tmp_build
mkdir tmp_build/examples
cp build/Release/gfunction_runner tmp_build/
cp -r examples tmp_build/examples

# create final package
mkdir release
/C/Program\ Files/7-zip/7z/exe a release/gfunction_Windows.zip ./tmp_build/*
