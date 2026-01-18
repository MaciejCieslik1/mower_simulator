/*
    Author: Hanna Biegacz

    Command to retrieve the current position of the mower.
    Implements ICommand interface.
*/

#pragma once
#include "ICommand.h"

class GetCurrentPositionCommand : public ICommand {
public:
    GetCurrentPositionCommand(double& outX, double& outY);
    bool execute(StateSimulation& sim, double dt) override;

    GetCurrentPositionCommand(const GetCurrentPositionCommand&) = delete;
    GetCurrentPositionCommand& operator=(const GetCurrentPositionCommand&) = delete;

private:
    double& out_x_;
    double& out_y_;
};
