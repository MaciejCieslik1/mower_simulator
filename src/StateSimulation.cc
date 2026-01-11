/* 
    Author: Maciej Cieslik
    
    Implements SimulationState class.

*/

#include <cmath>
#include <iostream>
#include "../include/Constants.h"
#include "../include/StateSimulation.h"
#include "../include/Point.h"
#include "../include/Lawn.h"
#include "../include/Logger.h"
#include "../include/MathHelper.h"
#include "../include/Exceptions.h"

using namespace std;


StateSimulation::StateSimulation(Lawn& lawn, Mover& mover, Logger& logger, FileLogger& file_logger) : lawn_(lawn),
    mover_(mover), logger_(logger), file_logger_(file_logger), time_(0), points_(vector<Point>()), next_point_id_(0) {}


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


const vector<Point>& StateSimulation::getPoints() const {
    return points_;
}


const unsigned int& StateSimulation::getNextPointId() const {
    return next_point_id_;
}


const FileLogger& StateSimulation::getFileLogger() const {
    return file_logger_;
}


void StateSimulation::simulateMovement(const double& distance) {
    double begginning_x = mover_.getX();
    double begginning_y = mover_.getY();
    short angle = mover_.getAngle();
    double optional_distance = distance;
    string message;

    try {
        mover_.move(distance, lawn_.getWidth(), lawn_.getLength());

        message = "Distance moved: " + to_string(distance) + "from point x: " + to_string(begginning_x) + 
            ", y: " + to_string(begginning_y);
    }
    catch (const MoveOutsideLawnError& e) {
        Log log = Log(time_, "Attempted to move outside the lawn.");
        logger_.push(log);
        file_logger_.saveLog(log);
        optional_distance = countDistanceToBorder(distance);
        mover_.move(optional_distance, lawn_.getWidth(), lawn_.getLength());

        message = "Distance moved to border: " + to_string(distance) + "from point x: " + 
            to_string(begginning_x) + ", y: " + to_string(begginning_y);
    }   

    Log log = Log(time_, message);
    file_logger_.saveLog(log);

    calculateMovementTime(optional_distance);

    if (mover_.getIsMowing()) {
        pair<double, double> beginning_point = pair<double, double>(begginning_x, begginning_y);
        pair<double, double> ending_point = pair<double, double>(mover_.getX(), mover_.getY());
        lawn_.cutGrassSection(beginning_point, mover_.getBladeDiameter(), ending_point, angle);
    }
    
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
    u_int64_t time = time_;
    string message;

    try {
        mover_.rotate(angle);

        message = "Rotated: " + to_string(angle) + " degrees.";
        
        calculateRotationTime(angle);
    }
    catch (const RotationAngleOutOfRangeError& e) {
        message = "Invalid angle. Rotation angle must be in [-360; 360] range.";

        logger_.push(Log(time, message));
    }

    Log log = Log(time, message);
    file_logger_.saveLog(log);
}


void StateSimulation::calculateRotationTime(const short& angle) {
    short MAX_ROTATION_ANGLE = 360;
    double SECONDS_TO_MILISECONDS_MULTIPLIER = 1000;
    short positive_angle = (angle + MAX_ROTATION_ANGLE) % MAX_ROTATION_ANGLE;

    double time_ms = positive_angle * SECONDS_TO_MILISECONDS_MULTIPLIER / Constants::ROTATION_SPEED;
    u_int64_t result_time = u_int64_t(ceil(time_ms / 10.0) * 10.0);

    time_ += result_time;
}


void StateSimulation::simulateMowingOptionOn() {
    mover_.turnOnMowing();

    string message = "Mowing mode: on";
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::simulateMowingOptionOff() {
    mover_.turnOffMowing();

    string message = "Mowing mode: off";
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::simulateAddPoint(const double& x, const double& y) {
    string message;

    if(lawn_.isPointInLawn(x, y)) {
        points_.push_back(Point(x, y, next_point_id_));

        message = "Added point with id: " + to_string(next_point_id_) + "on coordinates x: " + to_string(x) + 
            ", y: " + to_string(y);

        next_point_id_ ++;
    }
    else {
        message = "Unable to add point outside the lawn.";
        logger_.push(Log(time_, message));
    }

    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}

void StateSimulation::simulateDeletePoint(const unsigned int& id) {
    bool is_found = false;
    string message;

    for (auto iterator = points_.begin(); iterator != points_.end(); ) {
        if (iterator->getId() == id) {
            iterator = points_.erase(iterator);
            is_found = true;
        } 
        else {
            ++iterator;
        }
    }
    if (!is_found) {
        message = "Unable to delete point from lawn. Incorrect point's id: " + to_string(id);
        logger_.push(Log(time_, message));
    }
    else {
        message = "Deleted point with id: " + to_string(id);
    }

    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::simulateMovementToPoint(const unsigned int& id) {
    bool is_found = false;
    double x;
    double y;
    string message;

    for (const Point& point : points_) {
        if (point.getId() == id) {
            x = point.getX();
            y = point.getY();
            is_found = true;
            break;
        }
    }
    if (!is_found) {
        message = "Unable to find point in the lawn. Incorrect point's id " + to_string(id);
        Log log = Log(time_, message);

        logger_.push(log);
        file_logger_.saveLog(log);
        return;
    }

    while (abs(x - mover_.getX()) > Constants::DISTANCE_PRECISION || 
        abs(y - mover_.getY()) > Constants::DISTANCE_PRECISION) {
        moveToPointAttempt(x, y);
    }

    message = "Moving to point wiht id:  " + to_string(id);
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::moveToPointAttempt(const double& x, const double& y) {
    pair<short, double> rotation_distance = calculateAngleAndDistance(x, y);

    simulateRotation(rotation_distance.first);
    simulateMovement(rotation_distance.second);
}


pair<short, double> StateSimulation::calculateAngleAndDistance(const double& x, const double& y) const {
    double dx = x - mover_.getX();
    double dy = y - mover_.getY();
    short rotation;
    double distance;

    if (dx == 0) {
        rotation = calculateRotationNoDx(dy);
        distance = abs(dy);
    }   
    else {
        rotation = calculateRotationDx(dy, dx);
        distance = std::sqrt(dx*dx + dy*dy);
    }
    return pair<short, double>(rotation, distance);
}


double StateSimulation::calculateRotationNoDx(const double& dy) const {
    double rotation;

    if (dy > 0) {
        rotation = -mover_.getAngle();
    }
    else {
        rotation = 180 - mover_.getAngle();
    }
    return rotation;
}


double StateSimulation::calculateRotationDx(const double& dy, const double& dx) const {
    double target_angle_in_radians = atan2(dy, dx);
    double target_angle_degrees = MathHelper::convertRadiansToDegrees(target_angle_in_radians);

    short SQUARE_ANGLE = 90;
    short rotation = SQUARE_ANGLE - short(target_angle_degrees) - mover_.getAngle();

    return rotation;
}
