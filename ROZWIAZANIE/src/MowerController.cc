/*
    Author: Hanna Biegacz
    Implementation of MowerController class.
*/

#include "MowerController.h"

// Executes the front command in the queue. Commands run over multiple frames
// until they return true (finished). Only then does the queue move to the next command.
// This ensures commands execute in order without overlapping.
void MowerController::update(StateSimulation& sim, double dt) {
    if (command_queue_.empty()) {
        return;
    }

    if (command_queue_.front()->execute(sim, dt)) {
        command_queue_.pop();
    }
}

void MowerController::move(double cm) {
    command_queue_.push(std::make_unique<MoveCommand>(cm));
}

void MowerController::move(const double* distance_ptr, double scale) {
    command_queue_.push(std::make_unique<MoveCommand>(distance_ptr, scale));
}

void MowerController::rotate(short deg) {
    command_queue_.push(std::make_unique<RotateCommand>(deg));
}

void MowerController::setMowing(bool enable) {
    command_queue_.push(std::make_unique<MowingOptionCommand>(enable));
}

void MowerController::addPoint(double x, double y) {
    command_queue_.push(std::make_unique<AddPointCommand>(x, y));
}

void MowerController::deletePoint(unsigned int id) {
    command_queue_.push(std::make_unique<DeletePointCommand>(id));
}

void MowerController::moveToPoint(unsigned int point_id) {
    command_queue_.push(std::make_unique<MoveToPointCommand>(point_id));
}

void MowerController::getDistanceToPoint(unsigned int point_id, double& out_distance) {
    command_queue_.push(std::make_unique<GetDistanceToPointCommand>(point_id, out_distance));
}

void MowerController::rotateTowardsPoint(unsigned int point_id) {
    command_queue_.push(std::make_unique<RotateTowardsPointCommand>(point_id));
}

void MowerController::getCurrentAngle(unsigned short& out_angle) {
    command_queue_.push(std::make_unique<GetCurrentAngleCommand>(out_angle));
}

void MowerController::getCurrentPosition(double& out_x, double& out_y) {
    command_queue_.push(std::make_unique<GetCurrentPositionCommand>(out_x, out_y));
}
