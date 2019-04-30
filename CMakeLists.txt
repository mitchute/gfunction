cmake_minimum_required(VERSION 3.6)
project(gfunction)

include(CTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/gfunction.cpp src/gfunction.hpp third_party/nlohmann/json.hpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Clang Tidy
list(APPEND RUN_CLANG_TIDY_BIN_ARGS
        -clang-tidy-binary ${CLANG_TIDY_BIN}
        -header-filter=.*
        -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-misc-noexcept-move-constructor
        )

add_custom_target(
        tidy
        COMMAND ${RUN_CLANG_TIDY_BIN} ${RUN_CLANG_TIDY_BIN_ARGS}
        COMMENT "running clang tidy"
)

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

add_library(gfunction ${SOURCE_FILES})
add_executable(gfunction_runner src/main.cpp)
target_link_libraries(gfunction_runner gfunction)

set(TEST_SOURCE_FILES tests/main.cpp tests/gfunction.unit.cpp)
add_executable(gfunction_tests ${TEST_SOURCE_FILES})
target_link_libraries(gfunction_tests gfunction gtest)
