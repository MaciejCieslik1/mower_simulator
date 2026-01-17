#include "../include/MowerController.h"

void MowerController::move(double cm) {
    command_queue_.push(std::make_unique<MoveCommand>(cm));
}

void MowerController::move(const double* distance_ptr, double scale) {
    command_queue_.push(std::make_unique<MoveCommand>(distance_ptr, scale));
}

void MowerController::rotate(short deg) {
    command_queue_.push(std::make_unique<RotateCommand>(deg));
}

void MowerController::update(StateSimulation& sim, double dt) {
    if (command_queue_.empty()) {
        return;
    }

    if (command_queue_.front()->execute(sim, dt)) {
        command_queue_.pop();
    }
}

bool MowerController::hasCommands() const {
    return !command_queue_.empty();
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