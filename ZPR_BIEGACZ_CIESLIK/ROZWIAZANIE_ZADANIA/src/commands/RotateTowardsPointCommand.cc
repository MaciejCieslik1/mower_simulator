/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/RotateTowardsPointCommand.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

using namespace std;

RotateTowardsPointCommand::RotateTowardsPointCommand(unsigned int pointId)
    : point_id_(pointId) {}
    
// Rotates the mower to face a specific point. Runs over multiple frames
// until the mower is aligned with the target direction.
bool RotateTowardsPointCommand::execute(StateSimulation& sim, double dt) {
    if (!initialized_) {
        if (!initializeTarget(sim)) {
            return true; 
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
        ? min((double)rotationNeeded, max_step)
        : max((double)rotationNeeded, -max_step);
    
    rotation_accumulator_ += step;

    applyAccumulatedRotation(sim);
}

void RotateTowardsPointCommand::applyAccumulatedRotation(StateSimulation& sim) {
    if (abs(rotation_accumulator_) >= 1.0) {
        short actual_rot = static_cast<short>(rotation_accumulator_);
        sim.simulateRotation(actual_rot);
        rotation_accumulator_ -= actual_rot;
    }
}

bool RotateTowardsPointCommand::isAlignedWithTarget(short rotationNeeded) const {
    return abs(rotationNeeded) <= 2;
}
