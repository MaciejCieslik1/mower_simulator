/* 
    Author: Maciej Cieslik
    
    Declares all global constants that are initialized at runtime.
    These values are computed once at program start and accessible throughout the program.
*/

#pragma once

namespace Config {
    extern unsigned int MAX_BLADE_DIAMETER; // cm
    extern unsigned int MIN_BLADE_DIAMETER; // cm
    extern unsigned int MAX_MOVER_WIDTH; // cm
    extern unsigned int MIN_MOVER_WIDTH; // cm
    extern unsigned int MAX_MOVER_LENGTH; // cm
    extern unsigned int MIN_MOVER_LENGTH; // cm
    extern double FIELD_WIDTH; // cm
    extern unsigned int HORIZONTAL_FIELDS_NUMBER;
    extern unsigned int VERTICAL_FIELDS_NUMBER;
    extern unsigned int MIN_SPEED; // cm/s
    extern unsigned int MAX_SPEED; // cm/s 
    extern unsigned short STARTING_ANGLE; // degree (0-359), 0 meaning the mover is looking up
    extern double STARTING_X; // starting x coord of mover's central point
    extern double STARTING_Y; // starting x coord of mover's central point
    extern double MAX_HORIZONTAL_EXCEEDANCE; // max width of a mover's part, which is outside the lawn
    extern double MAX_VERTICAL_EXCEEDANCE; // max length of a mover's part, which is outside the lawn

    void initializeRuntimeConstants(const unsigned int& lawn_width, const unsigned int& lawn_length);
    void initializeMoverConstants(const unsigned int& mover_width, const unsigned int& mover_length, 
        const double& starting_x, const double& starting_y, const unsigned short& starting_angle);
}
