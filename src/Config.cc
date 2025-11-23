/* 
    Author: Maciej Cieslik
    
    Implements global constants computing at program start.
*/

#include <algorithm>
#include <cmath>
#include "../include/Constants.h"
#include "../include/Config.h"

using namespace std;

namespace Config {
    unsigned int MAX_BLADE_DIAMETER = 0;
    unsigned int MIN_BLADE_DIAMETER = 0;
    unsigned int MAX_MOVER_WIDTH = 0;
    unsigned int MIN_MOVER_WIDTH = 0;
    unsigned int MAX_MOVER_LENGTH = 0;
    unsigned int MIN_MOVER_LENGTH = 0;
    double FIELD_WIDTH = 0.0;
    unsigned int HORIZONTAL_FIELDS_NUMBER = 0;
    unsigned int VERTICAL_FIELDS_NUMBER = 0;
    unsigned int MIN_SPEED = 0;
    unsigned int MAX_SPEED = 0;

    void initializeRuntimeConstants(const unsigned int& lawn_width, const unsigned int& lawn_length) {

        const unsigned int ABSOLUTE_MIN_BLADE_DIAMETER = 10; // cm
        const unsigned int MIN_LAWN_DIVISION_FACTOR = 1000;
        MIN_BLADE_DIAMETER = max(ABSOLUTE_MIN_BLADE_DIAMETER, 
            min(lawn_width / MIN_LAWN_DIVISION_FACTOR, lawn_length / MIN_LAWN_DIVISION_FACTOR));

        const unsigned int ABSOLUTE_MAX_BLADE_DIAMETER = 100; // cm
        const unsigned int MAX_LAWN_DIVISION_FACTOR = 10;
        MAX_BLADE_DIAMETER = min(ABSOLUTE_MAX_BLADE_DIAMETER, 
            min(lawn_width / MAX_LAWN_DIVISION_FACTOR, lawn_length / MAX_LAWN_DIVISION_FACTOR));

        const unsigned int MOVER_SIZE_MULTIPLICATON_FACTOR = 2;
        MIN_MOVER_WIDTH = MIN_BLADE_DIAMETER;
        MAX_MOVER_WIDTH = MOVER_SIZE_MULTIPLICATON_FACTOR * MAX_BLADE_DIAMETER;
        MIN_MOVER_LENGTH = MIN_MOVER_WIDTH;
        MAX_MOVER_LENGTH = MAX_MOVER_WIDTH;
        
        FIELD_WIDTH = min(lawn_width, lawn_length) / 1000.0;

        HORIZONTAL_FIELDS_NUMBER = static_cast<unsigned int>(round(static_cast<double>(lawn_width) / FIELD_WIDTH));
        VERTICAL_FIELDS_NUMBER = static_cast<unsigned int>(round(static_cast<double>(lawn_length) / FIELD_WIDTH));

        const unsigned int ABSOLUTE_MIN_SPEED = 10;
        const unsigned int ABSOLUTE_MAX_SPEED = 1000;
        const unsigned int MIN_SPEED_DIVISION_FACTOR = 1000;
        const unsigned int MAX_SPEED_DIVISION_FACTOR = 10;
        
        MIN_SPEED = max(ABSOLUTE_MIN_SPEED, min(lawn_width / MIN_SPEED_DIVISION_FACTOR, 
            lawn_length / MIN_SPEED_DIVISION_FACTOR));
        MAX_SPEED = min(ABSOLUTE_MAX_SPEED, min(lawn_width / MAX_SPEED_DIVISION_FACTOR, 
            lawn_length / MAX_SPEED_DIVISION_FACTOR));
    }
}
