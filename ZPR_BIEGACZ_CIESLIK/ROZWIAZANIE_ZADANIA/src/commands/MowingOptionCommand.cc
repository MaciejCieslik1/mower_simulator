/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/MowingOptionCommand.h"

MowingOptionCommand::MowingOptionCommand(bool enable) : enable_(enable) {}

bool MowingOptionCommand::execute(StateSimulation& sim, double dt) {
    if (enable_) {
        sim.simulateMowingOptionOn();
    } else {
        sim.simulateMowingOptionOff();
    }
    return true; 
}
