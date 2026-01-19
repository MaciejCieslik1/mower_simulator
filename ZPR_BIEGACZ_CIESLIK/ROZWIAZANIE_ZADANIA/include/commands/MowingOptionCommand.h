/*
    Author: Hanna Biegacz

    Command to enable or disable the mowing functionality.
    Implements ICommand interface.
*/

#pragma once
#include "ICommand.h"

class MowingOptionCommand : public ICommand {
public:
    explicit MowingOptionCommand(bool enable);
    bool execute(StateSimulation& sim, double dt) override;

    MowingOptionCommand(const MowingOptionCommand&) = delete;
    MowingOptionCommand& operator=(const MowingOptionCommand&) = delete;
private:
    bool enable_;
};
