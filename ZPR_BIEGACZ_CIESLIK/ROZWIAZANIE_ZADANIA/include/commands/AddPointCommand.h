/*

    Author: Hanna Biegacz
    
    Command to add a new point to the simulation.
    Implements ICommand interface. Does not modify the world directly,
    but delegates the operation to StateSimulation object.
*/    

#pragma once
#include "ICommand.h"

class AddPointCommand : public ICommand {
public:
    AddPointCommand(double x, double y);
    bool execute(StateSimulation& sim, double dt) override;

    AddPointCommand(const AddPointCommand&) = delete;
    AddPointCommand& operator=(const AddPointCommand&) = delete;
private:
    double x_;
    double y_;
};
