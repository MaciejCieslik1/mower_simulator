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
    unsigned short STARTING_ANGLE = 0;
    double STARTING_X = 0.0;
    double STARTING_Y = 0.0;
    double MAX_HORIZONTAL_EXCEEDANCE = 0.0;
    double MAX_VERTICAL_EXCEEDANCE = 0.0;

    void initializeRuntimeConstants(const unsigned int& lawn_width, const unsigned int& lawn_length) {
        MIN_BLADE_DIAMETER = max(Constants::ABSOLUTE_MIN_BLADE_DIAMETER, 
            min(lawn_width / Constants::MIN_LAWN_DIVISION_FACTOR, lawn_length / Constants::MIN_LAWN_DIVISION_FACTOR));

        MAX_BLADE_DIAMETER = min(Constants::ABSOLUTE_MAX_BLADE_DIAMETER, 
            min(lawn_width / Constants::MAX_LAWN_DIVISION_FACTOR, lawn_length / Constants::MAX_LAWN_DIVISION_FACTOR));

        MIN_MOVER_WIDTH = MIN_BLADE_DIAMETER;
        MAX_MOVER_WIDTH = Constants::MOVER_SIZE_MULTIPLICATON_FACTOR * MAX_BLADE_DIAMETER;
        MIN_MOVER_LENGTH = MIN_MOVER_WIDTH;
        MAX_MOVER_LENGTH = MAX_MOVER_WIDTH;
        
        FIELD_WIDTH = min(lawn_width, lawn_length) / 1000.0;

        HORIZONTAL_FIELDS_NUMBER = static_cast<unsigned int>(round(static_cast<double>(lawn_width) / FIELD_WIDTH));
        VERTICAL_FIELDS_NUMBER = static_cast<unsigned int>(round(static_cast<double>(lawn_length) / FIELD_WIDTH));
        
        MIN_SPEED = max(Constants::ABSOLUTE_MIN_SPEED, min(lawn_width / Constants::MIN_SPEED_DIVISION_FACTOR, 
            lawn_length / Constants::MIN_SPEED_DIVISION_FACTOR));
        MAX_SPEED = min(Constants::ABSOLUTE_MAX_SPEED, min(lawn_width / Constants::MAX_SPEED_DIVISION_FACTOR, 
            lawn_length / Constants::MAX_SPEED_DIVISION_FACTOR));
    }

    void initializeMoverConstants(const unsigned int& mover_width, const unsigned int& mover_length, 
        const double& starting_x, const double& starting_y, const unsigned short& starting_angle) {
        MAX_HORIZONTAL_EXCEEDANCE = Constants::DISTANCE_PRECISION;
        MAX_VERTICAL_EXCEEDANCE = Constants::DISTANCE_PRECISION;
        STARTING_X = starting_x;
        STARTING_Y = starting_y;
        STARTING_ANGLE = starting_angle;
    }
}
