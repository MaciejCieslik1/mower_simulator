/*
    Author: Hanna Biegacz

    Implementation of a user command..
*/

#include "commands/AddPointCommand.h"

AddPointCommand::AddPointCommand(double x, double y) : x_(x), y_(y) {}

bool AddPointCommand::execute(StateSimulation& sim, double dt) {
    sim.simulateAddPoint(x_, y_);
    return true;
}
