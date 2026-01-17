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
    if (isRotationFinished()) {
        return true;
    }

    double step = calculateRotationStepForFrame(dt);
    updateInternalRotationState(step);
    applyAccumulatedRotationToSimulation(sim);
    return isRotationFinished();
}

double RotateCommand::calculateRotationStepForFrame(double dt) const {
    double max_rot_speed = static_cast<double>(Constants::ROTATION_SPEED);
    double max_step = max_rot_speed * dt;
    
    if (angle_left_ > 0) {
        return std::min(max_step, static_cast<double>(angle_left_));
    } else {
        return std::max(-max_step, static_cast<double>(angle_left_));
    }
}

void RotateCommand::updateInternalRotationState(double step) {
    rotation_accumulator_ += step;

    if (step > 0) {
        angle_left_ -= static_cast<short>(std::floor(step + 0.5)); // Zaokrąglanie
    } else {
        angle_left_ -= static_cast<short>(std::ceil(step - 0.5));
    }
}

void RotateCommand::applyAccumulatedRotationToSimulation(StateSimulation& sim) {
    if (std::abs(rotation_accumulator_) >= 1.0) {
        short actual_rot_to_apply = static_cast<short>(rotation_accumulator_);

        sim.simulateRotation(actual_rot_to_apply);

        rotation_accumulator_ -= actual_rot_to_apply;
    }
}

bool RotateCommand::isRotationFinished() const {
    return angle_left_ == 0 && std::abs(rotation_accumulator_) < 0.5;
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

GetDistanceToPointCommand::GetDistanceToPointCommand(unsigned int pointId, double& outDistance)
    : point_id_(pointId), out_distance_(outDistance) {}

bool GetDistanceToPointCommand::execute(StateSimulation& sim, double dt) {
    auto coords = sim.getPointCoordinates(point_id_);
    if (!coords) {
        sim.getFileLogger().saveMessage("Error: Point " + std::to_string(point_id_) + " not found for GetDistanceToPoint.");
        return true; 
    }

    double target_x = coords->first;
    double target_y = coords->second;
    double current_x = sim.getMower().getX();
    double current_y = sim.getMower().getY();
    double dx = target_x - current_x;
    double dy = target_y - current_y;
    double distance = std::sqrt(dx*dx + dy*dy);

    out_distance_ = distance;

    std::string msg = "Distance to point " + std::to_string(point_id_) + ": " + std::to_string(distance);
    sim.getFileLogger().saveMessage(msg);

    return true;
}

RotateTowardsPointCommand::RotateTowardsPointCommand(unsigned int pointId)
    : point_id_(pointId) {}

bool RotateTowardsPointCommand::execute(StateSimulation& sim, double dt) {
    if (!initialized_) {
        if (!initializeTarget(sim)) {
            return true; // Stop if point not found
        }
    }

    auto nav = sim.calculateNavigationVector(target_x_, target_y_);
    short rotation_needed = nav.first;

    if (rotation_needed != 0) {
        executeRotationLogic(sim, dt, rotation_needed);
    }

    if (isAlignedWithTarget(rotation_needed)) {
        return true;
    }

    return false;
}

bool RotateTowardsPointCommand::initializeTarget(StateSimulation& sim) {
    auto coords = sim.getPointCoordinates(point_id_);
    if (!coords) {
        return false;
    }
    target_x_ = coords->first;
    target_y_ = coords->second;
    initialized_ = true;
    return true;
}

void RotateTowardsPointCommand::executeRotationLogic(StateSimulation& sim, double dt, short rotationNeeded) {
    double rot_speed = static_cast<double>(Constants::ROTATION_SPEED);
    double max_step = rot_speed * dt;

    double step = (rotationNeeded > 0) 
        ? std::min((double)rotationNeeded, max_step)
        : std::max((double)rotationNeeded, -max_step);
    
    rotation_accumulator_ += step;

    applyAccumulatedRotation(sim);
}

void RotateTowardsPointCommand::applyAccumulatedRotation(StateSimulation& sim) {
    if (std::abs(rotation_accumulator_) >= 1.0) {
        short actual_rot = static_cast<short>(rotation_accumulator_);
        sim.simulateRotation(actual_rot);
        rotation_accumulator_ -= actual_rot;
    }
}

bool RotateTowardsPointCommand::isAlignedWithTarget(short rotationNeeded) const {
    return std::abs(rotationNeeded) <= 2;
}