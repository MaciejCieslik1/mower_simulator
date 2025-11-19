/* 
    Author: Maciej Cieslik
    
    Defines all global constants, which are known at the compile time.
*/

#pragma once

namespace Constants {
    inline constexpr unsigned int MAX_LAWN_WIDTH = 100000; // cm
    inline constexpr unsigned int MIN_LAWN_WIDTH = 100; // cm
    inline constexpr unsigned int MAX_LAWN_LENGTH = MAX_LAWN_WIDTH;
    inline constexpr unsigned int MIN_LAWN_LENGTH = MIN_LAWN_WIDTH;
    inline constexpr unsigned int LAWN_SIDES_MAX_RATIO = 10; 
    inline constexpr unsigned int LAWN_SIDE_PRECISION = 10; //cm
    inline constexpr double PI = 3.14159265358979;
}