/*
    Author: Hanna Biegacz
*/

#include "commands/GetCurrentAngleCommand.h"

GetCurrentAngleCommand::GetCurrentAngleCommand(unsigned short& output_angle) 
    : output_angle_(output_angle) {}

bool GetCurrentAngleCommand::execute(StateSimulation& sim, double dt) {
    output_angle_ = sim.getMower().getAngle();
    return true;
}
