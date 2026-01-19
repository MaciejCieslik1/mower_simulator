/*
    Author: Hanna Biegacz

    Command to retrieve the current angle of the mower.
    Implements ICommand interface. Does not modify the world directly,
    but retrieves information from StateSimulation object.
*/

#pragma once
#include "ICommand.h"

class GetCurrentAngleCommand : public ICommand {
private:
    unsigned short& output_angle_;

public:
    GetCurrentAngleCommand(unsigned short& output_angle);
    bool execute(StateSimulation& sim, double dt) override;
};
