/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/MoveCommand.h"
#include "Constants.h"
#include <string>

using namespace std;

MoveCommand::MoveCommand(double distance) 
    : distance_left_(distance), initialized_(true) {}

MoveCommand::MoveCommand(const double* distance_ptr, double scale)
    : distance_left_(0.0), deferred_distance_(distance_ptr), scale_(scale), initialized_(false) {}


// Moves the mower forward by a certain distance. Supports deferred initialization
// where the actual distance is calculated from a pointer value on first execution. 
// Example of a situation where this is useful is in main where we first calculate
// the total distance of the path and then pass a pointer to it to the MoveCommand.
bool MoveCommand::execute(StateSimulation& sim, double dt) {
    if (!initialized_) {
        if (deferred_distance_) {
            distance_left_ = (*deferred_distance_) * scale_;
        }
        initialized_ = true;
    }

    if (distance_left_ <= 0) return true;

    double speed = sim.getMower().getSpeed();
    double step = speed * dt;
    double actual_step = min(step, distance_left_);

    sim.simulateMovement(actual_step);
    distance_left_ -= actual_step;

    return distance_left_ <= Constants::DISTANCE_PRECISION;
}
