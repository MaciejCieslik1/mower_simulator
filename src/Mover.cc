/* 
    Author: Maciej Cieslik
    
    Implements mover class.
*/

#include <cmath>
#include "../include/Constants.h"
#include "../include/Config.h"
#include "../include/Lawn.h"
#include "../include/Mover.h"
#include "../include/MathHelper.h"
#include "../include/Exceptions.h"


using namespace std;


Mover::Mover(const unsigned int& width, const unsigned int& length, const unsigned int& blade_diameter,
        const unsigned int& speed) : width_(width), length_(length), blade_diameter_(blade_diameter), speed_(speed), 
        angle_(Config::STARTING_ANGLE), x_(Config::STARTING_X), y_(Config::STARTING_Y) {}


bool Mover::operator==(const Mover& other) const {
    return this->width_ == other.getWidth() && this->length_ == other.getLength() &&
        this->blade_diameter_ == other.getBladeDiameter() && this->speed_ == other.getSpeed() &&
        this->angle_ == other.getAngle() && std::fabs(this->x_ - other.getX()) < Constants::DISTANCE_PRECISION &&
           std::fabs(this->y_ - other.getY()) < Constants::DISTANCE_PRECISION;
}


bool Mover::operator!=(const Mover& other) const {
    return !((*this) == other);
}


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
    double calculated_x = calculatedPoint.first;
    double calculated_y = calculatedPoint.second;
    
    if (calculateIfXAccessible(calculated_x, lawn_width) && calculateIfYAccessible(calculated_y, lawn_length)) {
        setX(calculated_x);
        setY(calculated_y);
    }
    else {
        throw MoveOutsideLawnError("Attempted to move outside the lawn.");
    }
}


pair<double, double> Mover::calculateFinalPoint(const double& distance) const {
    double ROUND_MULTIPLIER = 1 / Constants::DISTANCE_PRECISION;
    const double ANGLE_IN_RADIANS = MathHelper::convertDegreesToRadians(getAngle());
    double calculated_x = getX() + sin(ANGLE_IN_RADIANS) * distance;
    double calculated_y = getY() + cos(ANGLE_IN_RADIANS) * distance;
    double rounded_x = MathHelper::roundNumber(calculated_x, ROUND_MULTIPLIER);
    double rounded_y = MathHelper::roundNumber(calculated_y, ROUND_MULTIPLIER);
    
    return pair<double, double>(rounded_x, rounded_y);
}


bool Mover::calculateIfXAccessible(const double& calculated_x, const unsigned int& lawn_width) const {
    return (calculated_x <= static_cast<double>(lawn_width) + Config::MAX_HORIZONTAL_EXCEEDANCE &&
        calculated_x >= -Config::MAX_HORIZONTAL_EXCEEDANCE);
}


bool Mover::calculateIfYAccessible(const double& calculated_y, const unsigned int& lawn_length) const {
    return (calculated_y <= static_cast<double>(lawn_length) + Config::MAX_VERTICAL_EXCEEDANCE &&
        calculated_y >= -Config::MAX_VERTICAL_EXCEEDANCE);
}


void Mover::rotate(const short& angle) {
    short MAX_ROTATION_ANGLE = 360;
    short MIN_ROTATION_ANGLE = -360;
    if (angle > MAX_ROTATION_ANGLE || angle < MIN_ROTATION_ANGLE) {
        throw RotationAngleOutOfRangeError("Ratation angle must be in [-360; 360] range.");
    }

    unsigned short calculated_angle = (getAngle() + angle + MAX_ROTATION_ANGLE) % MAX_ROTATION_ANGLE;
    setAngle(calculated_angle);
}
