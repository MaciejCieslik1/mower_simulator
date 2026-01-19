/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/MoveToPointCommand.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

using namespace std;

MoveToPointCommand::MoveToPointCommand(unsigned int pointId) 
    : point_id_(pointId) 
{}

// Navigates the mower to a specific point. Handles rotation towards the target
// and movement in that direction. Runs over multiple frames until arrival.
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

// Retrieves target point coordinates on first execution.
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
    return sqrt(dx*dx + dy*dy);
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
        ? min((double)rotationNeeded, max_step)
        : max((double)rotationNeeded, -max_step);
    
    rotation_accumulator_ += step;

    applyAccumulatedRotation(sim);
}

void MoveToPointCommand::applyAccumulatedRotation(StateSimulation& sim) {
    if (abs(rotation_accumulator_) >= 1.0) {
        short actual_rot = static_cast<short>(rotation_accumulator_);
        sim.simulateRotation(actual_rot);
        rotation_accumulator_ -= actual_rot;
    }
}

// Checks if the mower is facing the target within acceptable tolerance.
// Uses tighter tolerance when close to the target for precision.
bool MoveToPointCommand::isAlignedWithTarget(short rotationNeeded, double distanceToTarget) const {
    const double CLOSE_RANGE_THRESHOLD = 20.0;
    const short TIGHT_ANGLE_TOLERANCE = 2;
    const short LOOSE_ANGLE_TOLERANCE = 10;
    
    bool is_close_range = (distanceToTarget < CLOSE_RANGE_THRESHOLD);
    short angle_tolerance = is_close_range ? TIGHT_ANGLE_TOLERANCE : LOOSE_ANGLE_TOLERANCE;

    return abs(rotationNeeded) <= angle_tolerance;
}

void MoveToPointCommand::executeMovementLogic(StateSimulation& sim, double dt, double distanceToTarget) {
    double speed = sim.getMower().getSpeed();
    double move_step = speed * dt;
    double move_dist = min(distanceToTarget, move_step);
    
    sim.simulateMovement(move_dist);
}
