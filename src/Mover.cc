/* 
    Author: Maciej Cieslik
    
    Implements mover class.
*/

#include "../include/Config.h"
#include "../include/Mover.h"

using namespace std;


Mover::Mover(const unsigned int& width, const unsigned int& length, const unsigned int& blade_diameter,
        const unsigned int& speed) : width_(width), length_(length), blade_diameter_(blade_diameter), speed_(speed), 
        angle_(Config::STARTING_ANGLE), x_(Config::STARTING_X), y_(Config::STARTING_Y) {}


unsigned int Mover::getWidth() const {
    return width_;
}    


unsigned int Mover::getLength() const {
    return length_;
}


unsigned int Mover::getBladeDiameter() const {
    return blade_diameter_;
}


unsigned int Mover::getSpeed() const {
    return speed_;
}


unsigned short Mover::getAngle() const {
    return angle_;
}


double Mover::getX() const {
    return x_;
}


double Mover::getY() const {
    return y_;
}
