#include "../include/Commands.h"
#include "../include/Constants.h"
#include <cmath>
#include <algorithm>

MoveCommand::MoveCommand(double distance) : distance_left_(distance) {}

bool MoveCommand::execute(StateSimulation& sim, double dt) {
    if (distance_left_ <= 0) return true;

    double speed = sim.getMower().getSpeed();
    double step = speed * dt;
    double actual_step = std::min(step, distance_left_);

    sim.simulateMovement(actual_step);
    distance_left_ -= actual_step;

    return distance_left_ <= Constants::DISTANCE_PRECISION;
}

RotateCommand::RotateCommand(short angle) : angle_left_(angle) {}

bool RotateCommand::execute(StateSimulation& sim, double dt) {
    if (angle_left_ == 0) return true;

    double rot_speed = Constants::ROTATION_SPEED;
    double step = rot_speed * dt;
    
    short actual_step;
    if (angle_left_ > 0) {
        actual_step = static_cast<short>(std::min(step, static_cast<double>(angle_left_)));
        angle_left_ -= actual_step;
    } else {
        actual_step = static_cast<short>(std::max(-step, static_cast<double>(angle_left_)));
        angle_left_ -= actual_step;
    }

    sim.simulateRotation(actual_step);

    return angle_left_ == 0;
}

MowingOptionCommand::MowingOptionCommand(bool enable) : enable_(enable) {}

bool MowingOptionCommand::execute(StateSimulation& sim, double dt) {
    if (enable_) {
        sim.simulateMowingOptionOn();
    } else {
        sim.simulateMowingOptionOff();
    }
    return true; 
}

AddPointCommand::AddPointCommand(double x, double y) : x_(x), y_(y) {}

bool AddPointCommand::execute(StateSimulation& sim, double dt) {
    sim.simulateAddPoint(x_, y_);
    return true;
}

DeletePointCommand::DeletePointCommand(unsigned int id) : id_(id) {}

bool DeletePointCommand::execute(StateSimulation& sim, double dt) {
    sim.simulateDeletePoint(id_);
    return true;
}

MoveToPointCommand::MoveToPointCommand(unsigned int pointId) 
    : point_id_(pointId) 
{}

bool MoveToPointCommand::execute(StateSimulation& sim, double dt) {
    if (!initialized_) {
        if (!initializeTarget(sim)) {
            return true; 
        }
    }

    double distance = calculateDistanceToTarget(sim);
    if (hasArrivedAtTarget(sim, distance)) {
        return true; 
    }

    auto nav = sim.calculateNavigationVector(target_x_, target_y_);
    short rotation_needed = nav.first;

    if (rotation_needed != 0) {
        executeRotationLogic(sim, dt, rotation_needed);
    }

    if (!isAlignedWithTarget(rotation_needed, distance)) {
        return false; 
    }

    executeMovementLogic(sim, dt, distance);
    return false;
}

bool MoveToPointCommand::initializeTarget(StateSimulation& sim) {
    auto coords = sim.getPointCoordinates(point_id_);
    if (!coords) {
        return false;
    }
    target_x_ = coords->first;
    target_y_ = coords->second;
    initialized_ = true;
    return true;
}

double MoveToPointCommand::calculateDistanceToTarget(const StateSimulation& sim) const {
    double current_x = sim.getMower().getX();
    double current_y = sim.getMower().getY();
    double dx = target_x_ - current_x;
    double dy = target_y_ - current_y;
    return std::sqrt(dx*dx + dy*dy);
}

bool MoveToPointCommand::hasArrivedAtTarget(StateSimulation& sim, double currentDistance) const {
    if (currentDistance <= 3.0) {
        sim.logArrivalAtPoint(point_id_);
        return true;
    }
    return false;
}

void MoveToPointCommand::executeRotationLogic(StateSimulation& sim, double dt, short rotationNeeded) {
    double rot_speed = static_cast<double>(Constants::ROTATION_SPEED);
    double max_step = rot_speed * dt;

    double step = (rotationNeeded > 0) 
        ? std::min((double)rotationNeeded, max_step)
        : std::max((double)rotationNeeded, -max_step);
    
    rotation_accumulator_ += step;

    applyAccumulatedRotation(sim);
}

void MoveToPointCommand::applyAccumulatedRotation(StateSimulation& sim) {
    if (std::abs(rotation_accumulator_) >= 1.0) {
        short actual_rot = static_cast<short>(rotation_accumulator_);
        sim.simulateRotation(actual_rot);
        rotation_accumulator_ -= actual_rot;
    }
}

bool MoveToPointCommand::isAlignedWithTarget(short rotationNeeded, double distanceToTarget) const {
    bool is_close_range = (distanceToTarget < 20.0);
    short angle_tolerance = is_close_range ? 2 : 10;

    return std::abs(rotationNeeded) <= angle_tolerance;
}

void MoveToPointCommand::executeMovementLogic(StateSimulation& sim, double dt, double distanceToTarget) {
    double speed = sim.getMower().getSpeed();
    double move_step = speed * dt;
    double move_dist = std::min(distanceToTarget, move_step);
    
    sim.simulateMovement(move_dist);
}
