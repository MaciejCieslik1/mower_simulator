cmake_minimum_required(VERSION 3.10)

project(MinimalApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(mover_simulator src/main.cc)