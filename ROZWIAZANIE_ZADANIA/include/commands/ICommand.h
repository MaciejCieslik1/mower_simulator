/*
    Author: Hanna Biegacz

    Interface for a command.
    ICommand is an abstract base class for all commands.
    It defines the execute method that is called to execute the command.
    ICommand is used to execute commands in the simulation.
*/

#pragma once
#include "StateSimulation.h"


class ICommand {
public:
    ICommand() {} 
    virtual ~ICommand() = default;
    virtual bool execute(StateSimulation& sim, double dt) = 0;
    ICommand(const ICommand&) = delete;
    ICommand& operator=(const ICommand&) = delete;
};
