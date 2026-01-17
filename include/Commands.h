#pragma once

#include "StateSimulation.h"

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual bool execute(StateSimulation& sim, double dt) = 0;
};

class MoveCommand : public ICommand {
public:
    explicit MoveCommand(double distance);
    bool execute(StateSimulation& sim, double dt) override;

private:
    double distance_left_;
};

class RotateCommand : public ICommand {
public:
    explicit RotateCommand(short angle);
    bool execute(StateSimulation& sim, double dt) override;

private:
    short angle_left_;
};

class MowingOptionCommand : public ICommand {
public:
    explicit MowingOptionCommand(bool enable);
    bool execute(StateSimulation& sim, double dt) override;

private:
    bool enable_;
};

class AddPointCommand : public ICommand {
public:
    AddPointCommand(double x, double y);
    bool execute(StateSimulation& sim, double dt) override;

private:
    double x_;
    double y_;
};

class DeletePointCommand : public ICommand {
public:
    explicit DeletePointCommand(unsigned int id);
    bool execute(StateSimulation& sim, double dt) override;

private:
    unsigned int id_;
};