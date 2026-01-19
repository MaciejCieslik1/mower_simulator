/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/RotateCommand.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

using namespace std;

RotateCommand::RotateCommand(short angle) : angle_left_(angle) {}

// Rotates the mower by a specified angle over multiple frames.
// Uses an accumulator to handle smooth sub-degree rotation.
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
        return min(max_step, static_cast<double>(angle_left_));
    } else {
        return max(-max_step, static_cast<double>(angle_left_));
    }
}

void RotateCommand::updateInternalRotationState(double step) {
    constexpr double ROUNDING_OFFSET = 0.5; 
    rotation_accumulator_ += step;

    if (step > 0) {
        angle_left_ -= static_cast<short>(floor(step + ROUNDING_OFFSET)); 
    } else {
        angle_left_ -= static_cast<short>(ceil(step - ROUNDING_OFFSET));
    }
}

void RotateCommand::applyAccumulatedRotationToSimulation(StateSimulation& sim) {
    constexpr double MIN_DEGREE_THRESHOLD = 1.0;
    
    if (abs(rotation_accumulator_) >= MIN_DEGREE_THRESHOLD) {
        short actual_rot_to_apply = static_cast<short>(rotation_accumulator_);
        
        sim.simulateRotation(actual_rot_to_apply);
        rotation_accumulator_ -= actual_rot_to_apply;
    }
}

bool RotateCommand::isRotationFinished() const {
    constexpr short NO_ROTATION_LEFT = 0;
    constexpr double ROTATION_TOLERANCE = 0.5;
    
    return angle_left_ == NO_ROTATION_LEFT && abs(rotation_accumulator_) < ROTATION_TOLERANCE;
}
