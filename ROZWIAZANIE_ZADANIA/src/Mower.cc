/* 
    Author: Maciej Cieslik
    
    Implements mower class.
*/

#include <cmath>
#include "Constants.h"
#include "Config.h"
#include "Lawn.h"
#include "Mower.h"
#include "MathHelper.h"
#include "Exceptions.h"


using namespace std;


Mower::Mower(const unsigned int& width, const unsigned int& length, const unsigned int& blade_diameter,
        const unsigned int& speed) : width_(width), length_(length), blade_diameter_(blade_diameter), speed_(speed), 
        angle_(Config::STARTING_ANGLE), is_mowing_(true), x_(Config::STARTING_X), y_(Config::STARTING_Y) {}


bool Mower::operator==(const Mower& other) const {
    return this->width_ == other.getWidth() && this->length_ == other.getLength() &&
        this->blade_diameter_ == other.getBladeDiameter() && this->speed_ == other.getSpeed() &&
        this->angle_ == other.getAngle() && this->is_mowing_ == other.getIsMowing() && 
        std::fabs(this->x_ - other.getX()) < Constants::DISTANCE_PRECISION && 
        std::fabs(this->y_ - other.getY()) < Constants::DISTANCE_PRECISION;
}


bool Mower::operator!=(const Mower& other) const {
    return !((*this) == other);
}


unsigned int Mower::getWidth() const {
    return width_;
}    


unsigned int Mower::getLength() const {
    return length_;
}


unsigned int Mower::getBladeDiameter() const {
    return blade_diameter_;
}


unsigned int Mower::getSpeed() const {
    return speed_;
}


unsigned short Mower::getAngle() const {
    return angle_;
}


bool Mower::getIsMowing() const {
    return is_mowing_;
}


double Mower::getX() const {
    return x_;
}


double Mower::getY() const {
    return y_;
}


void Mower::setAngle(const unsigned short& newAngle) {
    angle_ = newAngle;
}


void Mower::setX(const double& newX) {
    x_ = newX;
}

void Mower::setY(const double& newY) {
    y_ = newY;
}


void Mower::move(const double& distance, const unsigned int& lawn_width, const unsigned int& lawn_length) {
    /* Change coord of mower to new ones calculated by using trygonometric functions. 
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


pair<double, double> Mower::calculateFinalPoint(const double& distance) const {
    // Calculate final point for mower movement

    double ROUND_MULTIPLIER = 1 / Constants::DISTANCE_PRECISION;
    const double ANGLE_IN_RADIANS = MathHelper::convertDegreesToRadians(getAngle());
    double calculated_x = getX() + sin(ANGLE_IN_RADIANS) * distance;
    double calculated_y = getY() + cos(ANGLE_IN_RADIANS) * distance;
    double rounded_x = MathHelper::roundNumber(calculated_x, ROUND_MULTIPLIER);
    double rounded_y = MathHelper::roundNumber(calculated_y, ROUND_MULTIPLIER);
    
    return pair<double, double>(rounded_x, rounded_y);
}


bool Mower::calculateIfXAccessible(const double& calculated_x, const unsigned int& lawn_width) const {
    // Calculate if X coord is accessible for mower

    return (calculated_x <= static_cast<double>(lawn_width) + Config::MAX_HORIZONTAL_EXCEEDANCE &&
        calculated_x >= -Config::MAX_HORIZONTAL_EXCEEDANCE);
}


bool Mower::calculateIfYAccessible(const double& calculated_y, const unsigned int& lawn_length) const {
    // Calculate if X coord is accessible for mower 

    return (calculated_y <= static_cast<double>(lawn_length) + Config::MAX_VERTICAL_EXCEEDANCE &&
        calculated_y >= -Config::MAX_VERTICAL_EXCEEDANCE);
}


void Mower::rotate(const short& angle) {
    // Rotate mower

    short MAX_ROTATION_ANGLE = 360;
    short MIN_ROTATION_ANGLE = -360;
    if (angle > MAX_ROTATION_ANGLE || angle < MIN_ROTATION_ANGLE) {
        throw RotationAngleOutOfRangeError("Ratation angle must be in [-360; 360] range.");
    }

    unsigned short calculated_angle = (getAngle() + angle + MAX_ROTATION_ANGLE) % MAX_ROTATION_ANGLE;
    setAngle(calculated_angle);
}


void Mower::turnOnMowing() {
    is_mowing_ = true;
}


void Mower::turnOffMowing() {
    is_mowing_ = false;
}
