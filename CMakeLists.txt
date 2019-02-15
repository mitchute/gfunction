cmake_minimum_required(VERSION 3.6)
project(gfunction)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/gfunction.cpp src/gfunction.hpp third_party/nlohmann/json.hpp)

add_executable(gfunction ${SOURCE_FILES})
