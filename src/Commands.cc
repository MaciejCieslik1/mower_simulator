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