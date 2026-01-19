/* 
    Author: Maciej Cieslik
    
    Implements SimulationState class.

*/

#include <cmath>
#include <iostream>
#include <optional>
#include "Constants.h"
#include "StateSimulation.h"
#include "Point.h"
#include "Lawn.h"
#include "Logger.h"
#include "MathHelper.h"
#include "Exceptions.h"

using namespace std;


StateSimulation::StateSimulation(Lawn& lawn, Mower& mower, Logger& logger, FileLogger& file_logger) : lawn_(lawn),
    mower_(mower), logger_(logger), file_logger_(file_logger), time_(0), points_(vector<Point>()), next_point_id_(0) {}


bool StateSimulation::operator==(const StateSimulation& other) const{
    return this->getLawn() == other.getLawn() && this->getMower() == other.getMower() && 
        this->getLogger().getLogs().size() == other.getLogger().getLogs().size() && 
        this->getTime() == other.getTime();
}


bool StateSimulation::operator!=(const StateSimulation& other) const{
    return !((*this) == other);
}


const Lawn& StateSimulation::getLawn() const {
    return lawn_;
}


const Mower& StateSimulation::getMower() const {
    return mower_;
}

Logger& StateSimulation::getLogger() {
    return logger_;
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
    /* Simulate movement of the mower. Handles situation when mower tries to go out of the lawn.
        Sends logs to file logger */

    double begginning_x = mower_.getX();
    double begginning_y = mower_.getY();
    short angle = mower_.getAngle();
    double optional_distance = distance;
    string message;

    try {
        mower_.move(distance, lawn_.getWidth(), lawn_.getLength());

        message = "Distance moved: " + to_string(distance) + "from point x: " + to_string(begginning_x) + 
            ", y: " + to_string(begginning_y);
    } catch (const MoveOutsideLawnError& e) {
        Log log = Log(time_, "Attempted to move outside the lawn.");
        logger_.push(log);
        file_logger_.saveLog(log);
        throw; 
    }

    Log log = Log(time_, message);
    file_logger_.saveLog(log);

    calculateMovementTime(optional_distance);

    if (mower_.getIsMowing()) {
        pair<double, double> beginning_point = pair<double, double>(begginning_x, begginning_y);
        pair<double, double> ending_point = pair<double, double>(mower_.getX(), mower_.getY());
        lawn_.cutGrassSection(beginning_point, mower_.getBladeDiameter(), ending_point, angle);
    }
    
}


double StateSimulation::countDistanceToBorder(const double& distance) const {
    // Count distance to the closest border of the lawn

    double ROUND_MULTIPLIER = 1 / Constants::DISTANCE_PRECISION;
    pair<double, double> borderCornerPoint = countBorderPoint();
    double border_x = borderCornerPoint.first;
    double border_y = borderCornerPoint.second;

    double angleInRadians = MathHelper::convertDegreesToRadians(mower_.getAngle());

    // distances below mean how long distance has mower to move in given direction to reach border or it's extension  
    double distance_to_vertical_border = (border_x - mower_.getX()) / sin(angleInRadians);
    double distance_to_horizontal_border = (border_y - mower_.getY()) / cos(angleInRadians);
    return min(MathHelper::roundNumber(distance_to_vertical_border, ROUND_MULTIPLIER), 
        MathHelper::roundNumber(distance_to_horizontal_border, ROUND_MULTIPLIER));
}


void StateSimulation::calculateMovementTime(const double& distance) {
    // Calculates time of movement action

    double SECONDS_TO_MILISECONDS_MULTIPLIER = 1000;

    double time_ms = distance * SECONDS_TO_MILISECONDS_MULTIPLIER / mower_.getSpeed();
    u_int64_t result_time = u_int64_t(ceil(time_ms / 10.0) * 10.0);

    time_ += result_time;
}


pair<double, double> StateSimulation::countBorderPoint() const {
    // Calculates coords of the point, in which mower must stop not to exeede teh lawn area

    double border_x;
    double border_y;

    if (mower_.getAngle() < 90) {
        border_x = lawn_.getWidth();
        border_y = lawn_.getLength();
    }
    else if (mower_.getAngle() < 180) {
        border_x = lawn_.getWidth();
        border_y = 0;
    }
    else if (mower_.getAngle() < 270) {
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
    /* Simulate rotation of the mower. Handles situation when mower wants to rotate incorrectly.
        Sends logs to file logger */

    short beginning_angle = mower_.getAngle();
    u_int64_t time = time_;
    string message;

    try {
        mower_.rotate(angle);

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
    // Calculate time of the rotation action

    short MAX_ROTATION_ANGLE = 360;
    double SECONDS_TO_MILISECONDS_MULTIPLIER = 1000;
    short positive_angle = (angle + MAX_ROTATION_ANGLE) % MAX_ROTATION_ANGLE;

    double time_ms = positive_angle * SECONDS_TO_MILISECONDS_MULTIPLIER / Constants::ROTATION_SPEED;
    u_int64_t result_time = u_int64_t(ceil(time_ms / 10.0) * 10.0);

    time_ += result_time;
}


void StateSimulation::simulateMowingOptionOn() {
    // Simulate switching on mowing

    mower_.turnOnMowing();

    string message = "Mowing mode: on";
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::simulateMowingOptionOff() {
    // Simulate switching off mowing

    mower_.turnOffMowing();

    string message = "Mowing mode: off";
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::simulateAddPoint(const double& x, const double& y) {
    // Simulate adding point on the law

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
}

void StateSimulation::simulateDeletePoint(const unsigned int& id) {
    // Simulates deleting point from the lawn

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
}


void StateSimulation::simulateMovementToPoint(const unsigned int& id) {
    /* Simulate movement to the point. Handles both moving and rotation.
        As it is impossible to move exactly to the given point with rotation tolerance, which equals 1 degree, 
        the mower is getting closer during each iteration so as to reach the given point*/

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

    while (abs(x - mower_.getX()) > Constants::DISTANCE_PRECISION || 
        abs(y - mower_.getY()) > Constants::DISTANCE_PRECISION) {
        moveToPointAttempt(x, y);
    }

    message = "Moving to point with id:  " + to_string(id);
    Log log = Log(time_, message);
    file_logger_.saveLog(log);
}


void StateSimulation::moveToPointAttempt(const double& x, const double& y) {
    /* Simulate attept to reach final point. The purpose of this method is to move closer to the lawn point,
        without confidence that the final point will be reached during this iteration*/ 

    pair<short, double> rotation_distance = calculateAngleAndDistance(x, y);

    simulateRotation(rotation_distance.first);
    simulateMovement(rotation_distance.second);
}


pair<short, double> StateSimulation::calculateAngleAndDistance(const double& x, const double& y) const {
    // Calculate angle and distance to the given point

    double dx = x - mower_.getX();
    double dy = y - mower_.getY();
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
    // Calculate rotation, when mower and point have the same x coord

    double rotation;

    if (dy > 0) {
        rotation = -mower_.getAngle();
    }
    else {
        rotation = 180 - mower_.getAngle();
    }
    return rotation;
}


double StateSimulation::calculateRotationDx(const double& dy, const double& dx) const {
    // Calculate rotation, when mower and point have the different x coord

    const short RIGHT_ANGLE = 90;
    const short HALF_CIRCLE = 180;
    const short FULL_CIRCLE = 360;

    double target_angle_in_radians = atan2(dy, dx);
    double target_angle_degrees = MathHelper::convertRadiansToDegrees(target_angle_in_radians);

    short rotation = RIGHT_ANGLE - short(target_angle_degrees) - mower_.getAngle();

    // Normalize angle to be in [-180, 180]
    while (rotation > HALF_CIRCLE) rotation -= FULL_CIRCLE;
    while (rotation < -HALF_CIRCLE) rotation += FULL_CIRCLE;

    return rotation;
}

SimulationSnapshot StateSimulation::buildSimulationSnapshot() const {
    SimulationSnapshot sim_snapshot;
    sim_snapshot.x_ = mower_.getX();
    sim_snapshot.y_ = mower_.getY();
    sim_snapshot.angle_ = mower_.getAngle();
    sim_snapshot.simulation_time_ = static_cast<double>(time_);

    sim_snapshot.fields_ = lawn_.getFields();
    sim_snapshot.points_ = points_;

    return sim_snapshot;
}
 
StaticSimulationData StateSimulation::getStaticData() const {
    StaticSimulationData data;
    data.lawn_width_ = lawn_.getWidth();
    data.lawn_length_ = lawn_.getLength();
    data.width_cm_ = mower_.getWidth();
    data.length_cm = mower_.getLength();
    data.blade_diameter_cm = mower_.getBladeDiameter();
    return data;
}


std::optional<std::pair<double, double>> StateSimulation::getPointCoordinates(unsigned int pointId) {
    for (const Point& point : points_) {
        if (point.getId() == pointId) {
            return std::make_pair(point.getX(), point.getY());
        }
    }
    
    string message = "Unable to find point in the lawn. Incorrect point's id " + to_string(pointId);
    Log log = Log(time_, message);
    logger_.push(log);
    file_logger_.saveLog(log);
    
    return std::nullopt;
}

void StateSimulation::logArrivalAtPoint(unsigned int pointId) {
    string message = "Arrived at point with id:  " + to_string(pointId);
    Log log = Log(time_, message);
    logger_.push(log);       
}

std::pair<short, double> StateSimulation::calculateNavigationVector(double targetX, double targetY) const {
    return calculateAngleAndDistance(targetX, targetY);
}
