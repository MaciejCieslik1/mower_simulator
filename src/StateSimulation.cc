/* 
    Author: Maciej Cieslik
    
    Implements SimulationState class.

*/

#include <cmath>
#include <iostream>
#include "../include/Constants.h"
#include "../include/StateSimulation.h"
#include "../include/Lawn.h"
#include "../include/Logger.h"
#include "../include/MathHelper.h"
#include "../include/Exceptions.h"

using namespace std;


StateSimulation::StateSimulation(Lawn& lawn, Mover& mover, Logger& logger) : lawn_(lawn),
    mover_(mover), logger_(logger), time_(0) {}


bool StateSimulation::operator==(const StateSimulation& other) const{
    return this->getLawn() == other.getLawn() && this->getMover() == other.getMover() && 
        this->getLogger().getLogs().size() == other.getLogger().getLogs().size() && 
        this->getTime() == other.getTime();
}


bool StateSimulation::operator!=(const StateSimulation& other) const{
    return !((*this) == other);
}


const Lawn& StateSimulation::getLawn() const {
    return lawn_;
}


const Mover& StateSimulation::getMover() const {
    return mover_;
}


const Logger& StateSimulation::getLogger() const {
    return logger_;
}


const u_int64_t& StateSimulation::getTime() const {
    return time_;
}


void StateSimulation::simulateMovement(const double& distance) {
    double begginning_x = mover_.getX();
    double begginning_y = mover_.getY();
    short angle = mover_.getAngle();
    double optional_distance = distance;

    try {
        mover_.move(distance, lawn_.getWidth(), lawn_.getLength());
    }
    catch (const MoveOutsideLawnError& e) {
        logger_.push(Log(time_, "Attempted to move outside the lawn."));
        optional_distance = countDistanceToBorder(distance);
        mover_.move(optional_distance, lawn_.getWidth(), lawn_.getLength());
    }   
    calculateMovementTime(optional_distance);
    pair<double, double> beginning_point = pair<double, double>(begginning_x, begginning_y);
    pair<double, double> ending_point = pair<double, double>(mover_.getX(), mover_.getY());
    lawn_.cutGrassSection(beginning_point, mover_.getBladeDiameter(), ending_point, angle);
}


double StateSimulation::countDistanceToBorder(const double& distance) const {
    double ROUND_MULTIPLIER = 1 / Constants::DISTANCE_PRECISION;
    pair<double, double> borderCornerPoint = countBorderPoint();
    double border_x = borderCornerPoint.first;
    double border_y = borderCornerPoint.second;

    double angleInRadians = MathHelper::convertDegreesToRadians(mover_.getAngle());

    // distances below mean how long distance has mower to move in given direction to reach border or it's extension  
    double distance_to_vertical_border = (border_x - mover_.getX()) / sin(angleInRadians);
    double distance_to_horizontal_border = (border_y - mover_.getY()) / cos(angleInRadians);
    return min(MathHelper::roundNumber(distance_to_vertical_border, ROUND_MULTIPLIER), 
        MathHelper::roundNumber(distance_to_horizontal_border, ROUND_MULTIPLIER));
}


void StateSimulation::calculateMovementTime(const double& distance) {
    double SECONDS_TO_MILISECONDS_MULTIPLIER = 1000;

    double time_ms = distance * SECONDS_TO_MILISECONDS_MULTIPLIER / mover_.getSpeed();
    u_int64_t result_time = u_int64_t(ceil(time_ms / 10.0) * 10.0);

    time_ += result_time;
}


pair<double, double> StateSimulation::countBorderPoint() const {
    double border_x;
    double border_y;

    if (mover_.getAngle() < 90) {
        border_x = lawn_.getWidth();
        border_y = lawn_.getLength();
    }
    else if (mover_.getAngle() < 180) {
        border_x = lawn_.getWidth();
        border_y = 0;
    }
    else if (mover_.getAngle() < 270) {
        border_x = 0;
        border_y = 0;
    }
    else {
        border_x = 0;
        border_y = lawn_.getLength();
    }
    return pair<double, double>(border_x, border_y);
}


void StateSimulation::simulateRotation(const short& angle) {
    short beginning_angle = mover_.getAngle();
    try {
        mover_.rotate(angle);
        calculateRotationTime(angle);
    }
    catch (const RotationAngleOutOfRangeError& e) {
        logger_.push(Log(time_, "Rotation angle must be in [-360; 360] range."));
    }
}


void StateSimulation::calculateRotationTime(const short& angle) {
    short MAX_ROTATION_ANGLE = 360;
    double SECONDS_TO_MILISECONDS_MULTIPLIER = 1000;
    short positive_angle = (angle + MAX_ROTATION_ANGLE) % MAX_ROTATION_ANGLE;

    double time_ms = positive_angle * SECONDS_TO_MILISECONDS_MULTIPLIER / Constants::ROTATION_SPEED;
    u_int64_t result_time = u_int64_t(ceil(time_ms / 10.0) * 10.0);

    time_ += result_time;
}
