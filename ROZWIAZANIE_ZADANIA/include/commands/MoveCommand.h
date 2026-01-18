/*
    Author: Hanna Biegacz

    Command to move the mower by a specified distance.
    Implements ICommand interface.
    Supports both immediate and deferred distance calculation with scaling.
*/


#pragma once
#include "ICommand.h"

class MoveCommand : public ICommand {
public:
    explicit MoveCommand(double distance);
    MoveCommand(const double* distance_ptr, double scale);
    bool execute(StateSimulation& sim, double dt) override;

    MoveCommand(const MoveCommand&) = delete;
    MoveCommand& operator=(const MoveCommand&) = delete;

private:
    double distance_left_;
    const double* deferred_distance_ = nullptr;
    double scale_ = 1.0;
    bool initialized_ = false;
};
