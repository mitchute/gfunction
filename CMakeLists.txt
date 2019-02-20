cmake_minimum_required(VERSION 3.6)
project(gfunction)

include(CTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/gfunction.cpp src/gfunction.hpp third_party/nlohmann/json.hpp)

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

add_executable(gfunction ${SOURCE_FILES})
target_link_libraries(example gtest gmock_main)
