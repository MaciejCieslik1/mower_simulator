/* 
    Author: Maciej Cieslik
    
    Implements mover class.
*/

#include <cmath>
#include "../include/Constants.h"
#include "../include/Config.h"
#include "../include/Lawn.h"
#include "../include/Mover.h"
#include "../include/Exceptions.h"


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


void Mover::setAngle(const unsigned short& newAngle) {
    angle_ = newAngle;
}


void Mover::setX(const double& newX) {
    x_ = newX;
}

void Mover::setY(const double& newY) {
    y_ = newY;
}


void Mover::move(const double& distance, const unsigned int& lawn_width, const unsigned int& lawn_length) {
    /* Changes coord of mower to new ones calculated by using trygonometric functions. 
        Throws MoveOutsideLawnError when destination point(the middle of the mower) is outside the lawn */

    pair<double, double>(calculatedPoint) = calculateFinalPoint(distance);
    double calculatedX = calculatedPoint.first;
    double calculatedY = calculatedPoint.second;
    
    if (calculateIfXAccessible(calculatedX, lawn_width) && calculateIfYAccessible(calculatedY, lawn_length)) {
        setX(calculatedX);
        setY(calculatedY);
    }
    else {
        throw MoveOutsideLawnError("Attempted to move outside the lawn.");
    }
}


pair<double, double> Mover::calculateFinalPoint(const double& distance) const {
    const double ANGLE_IN_RADIANS = convertDegreesToRadians();
    double calculatedX = getX() + sin(ANGLE_IN_RADIANS) * distance;
    double calculatedY = getY() + cos(ANGLE_IN_RADIANS) * distance;
    double roundedX = round(calculatedX / Constants::DISTANCE_PRECISION) * Constants::DISTANCE_PRECISION;
    double roundedY = round(calculatedY / Constants::DISTANCE_PRECISION) * Constants::DISTANCE_PRECISION;
    
    return pair<double, double>(roundedX, roundedY);
}


double Mover::convertDegreesToRadians() const {
    double RADIAN_FACTOR = 180.0;
    const double DEGREE_TO_RADIAN_FACTOR = Constants::PI / RADIAN_FACTOR;
    double radians = angle_ * DEGREE_TO_RADIAN_FACTOR;
    return radians;
}


bool Mover::calculateIfXAccessible(const double& calculatedX, const unsigned int& lawn_width) const {
    return (calculatedX <= static_cast<double>(lawn_width) + Config::MAX_HORIZONTAL_EXCEEDANCE &&
        calculatedX >= -Config::MAX_HORIZONTAL_EXCEEDANCE);
}


bool Mover::calculateIfYAccessible(const double& calculatedY, const unsigned int& lawn_length) const {
    return (calculatedY <= static_cast<double>(lawn_length) + Config::MAX_VERTICAL_EXCEEDANCE &&
        calculatedY >= -Config::MAX_VERTICAL_EXCEEDANCE);
}
