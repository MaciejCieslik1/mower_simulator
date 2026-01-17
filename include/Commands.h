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

class MoveToPointCommand : public ICommand {
public:
    explicit MoveToPointCommand(unsigned int pointId);
    bool execute(StateSimulation& sim, double dt) override;

private:
    unsigned int point_id_;
        
    bool initialized_ = false;
    double target_x_ = 0.0;
    double target_y_ = 0.0;
    double rotation_accumulator_ = 0.0;

    bool initializeTarget(StateSimulation& sim);
    double calculateDistanceToTarget(const StateSimulation& sim) const;
    bool hasArrivedAtTarget(StateSimulation& sim, double currentDistance) const;
    void executeRotationLogic(StateSimulation& sim, double dt, short rotationNeeded);
    void applyAccumulatedRotation(StateSimulation& sim);
    void executeMovementLogic(StateSimulation& sim, double dt, double distanceToTarget);
    bool isAlignedWithTarget(short rotationNeeded, double distanceToTarget) const;
};
