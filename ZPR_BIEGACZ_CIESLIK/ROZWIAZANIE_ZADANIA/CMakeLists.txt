cmake_minimum_required(VERSION 3.10)
project(MowerSimulator VERSION 1.0 LANGUAGES CXX)
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

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

add_executable(mower_simulator src/Main.cc src/Config.cc src/Mower.cc src/Lawn.cc src/Exceptions.cc src/Visualizer.cc include/Visualizer.h src/Engine.cc src/Log.cc src/Logger.cc src/StateSimulation.cc src/MathHelper.cc src/Point.cc src/FileLogger.cc src/StateInterpolator.cc src/RenderTimeController.cc src/MowerController.cc src/commands/MoveCommand.cc src/commands/RotateCommand.cc src/commands/MowingOptionCommand.cc src/commands/AddPointCommand.cc src/commands/DeletePointCommand.cc src/commands/MoveToPointCommand.cc src/commands/GetDistanceToPointCommand.cc src/commands/RotateTowardsPointCommand.cc src/commands/GetCurrentAngleCommand.cc src/commands/GetCurrentPositionCommand.cc)

add_definitions(-DASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/include/assets")
target_link_libraries(mower_simulator Qt5::Widgets  Threads::Threads)

# Tests
add_executable(ConfigTests tests/ConfigTests.cc src/Config.cc src/Exceptions.cc)
target_link_libraries(ConfigTests gtest gtest_main pthread)
add_test(NAME ConfigTests COMMAND ConfigTests)

add_executable(LawnTests tests/LawnTests.cc src/Lawn.cc src/Config.cc src/Exceptions.cc src/MathHelper.cc)
target_link_libraries(LawnTests gtest gtest_main pthread)
add_test(NAME LawnTests COMMAND LawnTests)

add_executable(PointTests tests/PointTests.cc src/Point.cc src/Exceptions.cc)
target_link_libraries(PointTests gtest gtest_main)
add_test(NAME PointTests COMMAND PointTests)

add_executable(MowerTests tests/MowerTests.cc src/Mower.cc src/Config.cc src/Exceptions.cc src/MathHelper.cc) 
target_link_libraries(MowerTests gtest gtest_main)
add_test(NAME MowerTests COMMAND MowerTests)

add_executable(VisualizerTests tests/VisualizerTests.cc src/Visualizer.cc include/Visualizer.h src/Lawn.cc src/Config.cc src/MathHelper.cc src/StateSimulation.cc src/Mower.cc src/Logger.cc src/Log.cc src/Point.cc src/FileLogger.cc src/Exceptions.cc src/Engine.cc src/StateInterpolator.cc src/RenderTimeController.cc)
target_link_libraries(VisualizerTests gtest gtest_main pthread Qt5::Widgets Threads::Threads)
add_test(NAME VisualizerTests COMMAND VisualizerTests)

add_executable(LogTests tests/LogTests.cc src/Log.cc) 
target_link_libraries(LogTests gtest gtest_main)
add_test(NAME LogTests COMMAND LogTests)

add_executable(LoggerTests tests/LoggerTests.cc src/Logger.cc src/Log.cc) 
target_link_libraries(LoggerTests gtest gtest_main)
add_test(NAME LoggerTests COMMAND LoggerTests)

add_executable(StateSimulationTests tests/StateSimulationTests.cc src/Logger.cc src/Log.cc src/Lawn.cc src/Mower.cc src/StateSimulation.cc src/Exceptions.cc src/Config.cc src/MathHelper.cc src/Point.cc src/FileLogger.cc) 
target_link_libraries(StateSimulationTests gtest gtest_main)
add_test(NAME StateSimulationTests COMMAND StateSimulationTests)

add_executable(EngineTests tests/EngineTests.cc src/Engine.cc src/StateSimulation.cc src/Lawn.cc src/Mower.cc src/Logger.cc src/Log.cc src/Config.cc src/Exceptions.cc src/MathHelper.cc src/Point.cc src/FileLogger.cc src/Visualizer.cc include/Visualizer.h src/StateInterpolator.cc src/RenderTimeController.cc src/MowerController.cc src/commands/MoveCommand.cc src/commands/RotateCommand.cc src/commands/MowingOptionCommand.cc src/commands/AddPointCommand.cc src/commands/DeletePointCommand.cc src/commands/MoveToPointCommand.cc src/commands/GetDistanceToPointCommand.cc src/commands/RotateTowardsPointCommand.cc src/commands/GetCurrentAngleCommand.cc src/commands/GetCurrentPositionCommand.cc)
target_link_libraries(EngineTests gtest gtest_main pthread Threads::Threads Qt5::Widgets)
add_test(NAME EngineTests COMMAND EngineTests)

add_executable(StateInterpolatorTests tests/StateInterpolatorTests.cc src/StateInterpolator.cc src/Point.cc src/MathHelper.cc)
target_link_libraries(StateInterpolatorTests gtest gtest_main pthread)
add_test(NAME StateInterpolatorTests COMMAND StateInterpolatorTests)

add_executable(RenderTimeControllerTests tests/RenderTimeControllerTests.cc src/RenderTimeController.cc src/StateInterpolator.cc src/Point.cc src/MathHelper.cc)
target_link_libraries(RenderTimeControllerTests gtest gtest_main pthread)
add_test(NAME RenderTimeControllerTests COMMAND RenderTimeControllerTests)

add_executable(CommandTests tests/CommandTests.cc src/commands/MoveCommand.cc src/commands/RotateCommand.cc src/commands/MowingOptionCommand.cc src/commands/AddPointCommand.cc src/commands/DeletePointCommand.cc src/commands/MoveToPointCommand.cc src/commands/GetDistanceToPointCommand.cc src/commands/RotateTowardsPointCommand.cc src/StateSimulation.cc src/Lawn.cc src/Mower.cc src/Config.cc src/Exceptions.cc src/MathHelper.cc src/Point.cc src/Logger.cc src/Log.cc src/FileLogger.cc src/commands/GetCurrentAngleCommand.cc src/commands/GetCurrentPositionCommand.cc)
target_link_libraries(CommandTests gtest gtest_main pthread)
add_test(NAME CommandTests COMMAND CommandTests)

add_executable(MowerControllerTests tests/MowerControllerTests.cc src/MowerController.cc src/commands/MoveCommand.cc src/commands/RotateCommand.cc src/commands/MowingOptionCommand.cc src/commands/AddPointCommand.cc src/commands/DeletePointCommand.cc src/commands/MoveToPointCommand.cc src/commands/GetDistanceToPointCommand.cc src/commands/RotateTowardsPointCommand.cc src/StateSimulation.cc src/Lawn.cc src/Mower.cc src/Config.cc src/Exceptions.cc src/MathHelper.cc src/Point.cc src/Logger.cc src/Log.cc src/FileLogger.cc src/commands/GetCurrentAngleCommand.cc src/commands/GetCurrentPositionCommand.cc)
target_link_libraries(MowerControllerTests gtest gtest_main pthread)
add_test(NAME MowerControllerTests COMMAND MowerControllerTests)

