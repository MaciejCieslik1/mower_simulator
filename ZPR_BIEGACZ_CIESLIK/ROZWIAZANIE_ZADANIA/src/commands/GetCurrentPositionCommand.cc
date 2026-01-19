/*
    Author: Hanna Biegacz

    Implementation of a user command to get the current mower position.
*/

#include "commands/GetCurrentPositionCommand.h"
#include <string>

GetCurrentPositionCommand::GetCurrentPositionCommand(double& outX, double& outY)
    : out_x_(outX), out_y_(outY) {}

bool GetCurrentPositionCommand::execute(StateSimulation& sim, double dt) {
    out_x_ = sim.getMower().getX();
    out_y_ = sim.getMower().getY();

    std::string msg = "Current position: (" + std::to_string(out_x_) + ", " + std::to_string(out_y_) + ")";
    sim.getFileLogger().saveMessage(msg);

    return true;
}
