cmake_minimum_required(VERSION 3.10)
project(MowerSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

enable_testing()

# --- Qt config  ---
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Threads REQUIRED)


# --- Include directories ---
include_directories(${PROJECT_SOURCE_DIR}/include)

# --- Google Test ---
add_subdirectory(libs/googletest)
include_directories(libs/googletest/googletest/include)

add_executable(mower_simulator src/Main.cc src/Config.cc src/Lawn.cc src/simulation/LawnSimulationView.cc include/simulation/LawnSimulationView.h src/simulation/SimulationEngine.cc)
target_link_libraries(mower_simulator Qt5::Widgets  Threads::Threads)


# Tests
add_executable(ConfigTests tests/ConfigTests.cc src/Config.cc)
target_link_libraries(ConfigTests gtest gtest_main pthread)
add_test(NAME ConfigTests COMMAND ConfigTests)

add_executable(LawnTests tests/LawnTests.cc src/Lawn.cc src/Config.cc)
target_link_libraries(LawnTests gtest gtest_main pthread)
add_test(NAME LawnTests COMMAND LawnTests)

add_executable(PointTests tests/PointTests.cc src/Point.cc)
target_link_libraries(PointTests gtest gtest_main)
add_test(NAME PointTests COMMAND PointTests)

add_executable(SimulationTests tests/SimulationTests.cc src/simulation/SimulationEngine.cc src/Lawn.cc src/Config.cc)
target_link_libraries(SimulationTests gtest gtest_main pthread Threads::Threads)
add_test(NAME SimulationTests COMMAND SimulationTests)

add_executable(LawnSimulationViewTests tests/LawnSimulationViewTests.cc src/simulation/LawnSimulationView.cc include/simulation/LawnSimulationView.h src/Lawn.cc src/Config.cc)
target_link_libraries(LawnSimulationViewTests gtest gtest_main pthread Qt5::Widgets Threads::Threads)
add_test(NAME LawnSimulationViewTests COMMAND LawnSimulationViewTests)