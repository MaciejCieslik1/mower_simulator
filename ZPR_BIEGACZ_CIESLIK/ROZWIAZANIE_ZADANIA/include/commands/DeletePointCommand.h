#pragma once
#include "ICommand.h"

class DeletePointCommand : public ICommand {
public:
    explicit DeletePointCommand(unsigned int id);
    bool execute(StateSimulation& sim, double dt) override;

    DeletePointCommand(const DeletePointCommand&) = delete;
    DeletePointCommand& operator=(const DeletePointCommand&) = delete;
private:
    unsigned int id_;
};
