/* 
    Author: Maciej Cieslik
    
    Defines all global constants, which are known at the compile time.
*/

#pragma once

namespace Constants {
    inline constexpr unsigned int MAX_LAWN_WIDTH = 10000; // cm
    inline constexpr unsigned int MIN_LAWN_WIDTH = 100; // cm
    inline constexpr unsigned int MAX_LAWN_LENGTH = MAX_LAWN_WIDTH;
    inline constexpr unsigned int MIN_LAWN_LENGTH = MIN_LAWN_WIDTH;
    inline constexpr unsigned int LAWN_SIDES_MAX_RATIO = 10; 
    inline constexpr unsigned int LAWN_SIDE_PRECISION = 10; //cm
    inline constexpr unsigned int ABSOLUTE_MIN_BLADE_DIAMETER = 10; // cm
    inline constexpr unsigned int MIN_LAWN_DIVISION_FACTOR = 100;
    inline constexpr unsigned int ABSOLUTE_MAX_BLADE_DIAMETER = 100; // cm
    inline constexpr unsigned int MAX_LAWN_DIVISION_FACTOR = 10;
    inline constexpr unsigned int MOVER_SIZE_MULTIPLICATON_FACTOR = 2;
    inline constexpr unsigned int ABSOLUTE_MIN_SPEED = 10; // cm/s
    inline constexpr unsigned int ABSOLUTE_MAX_SPEED = 1000; // cm/s
    inline constexpr unsigned int MIN_SPEED_DIVISION_FACTOR = 1000;
    inline constexpr unsigned int MAX_SPEED_DIVISION_FACTOR = 10;
    inline constexpr double DISTANCE_PRECISION = 0.001; // cm
    inline constexpr u_int64_t TICK_DURATION = 10; // ms
    inline constexpr unsigned int ROTATION_SPEED = 90; // degrees / s
    
    inline constexpr double PI = 3.14159265358979;
}
