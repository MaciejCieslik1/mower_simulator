cmake_minimum_required(VERSION 3.10)
project(MoverSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

# --- Google Test ---
add_subdirectory(libs/googletest)
include_directories(libs/googletest/googletest/include)

add_executable(mover_simulator src/Main.cc src/Config.cc)

# Tests
add_executable(ConfigTests tests/ConfigTests.cc src/Config.cc)
target_link_libraries(ConfigTests gtest gtest_main pthread)
add_test(NAME ConfigTests COMMAND ConfigTests)

add_executable(LawnTests tests/LawnTests.cc src/Lawn.cc src/Config.cc)
target_link_libraries(LawnTests gtest gtest_main pthread)
add_test(NAME LawnTests COMMAND LawnTests)
