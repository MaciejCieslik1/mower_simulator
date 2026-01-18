/*
    Author: Hanna Biegacz

    Command to delete an existing point from the simulation.
    Implements ICommand interface. Does not modify the world directly,
    but delegates the operation to StateSimulation object.
*/

#include "commands/DeletePointCommand.h"

DeletePointCommand::DeletePointCommand(unsigned int id) : id_(id) {}

bool DeletePointCommand::execute(StateSimulation& sim, double dt) {
    sim.simulateDeletePoint(id_);
    return true;
}
