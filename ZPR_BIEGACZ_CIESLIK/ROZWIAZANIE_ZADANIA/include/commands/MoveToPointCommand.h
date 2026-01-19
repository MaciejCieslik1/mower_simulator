/*
    Author: Hanna Biegacz

    Command to navigate the mower to a specific point.
    Implements ICommand interface.
    Executes over multiple frames: first rotates the mower to face the target,
    then moves forward until arrival.
*/

#pragma once
#include "ICommand.h"

class MoveToPointCommand : public ICommand {
public:
    explicit MoveToPointCommand(unsigned int pointId);
    bool execute(StateSimulation& sim, double dt) override;

    MoveToPointCommand(const MoveToPointCommand&) = delete;
    MoveToPointCommand& operator=(const MoveToPointCommand&) = delete;
private:
    unsigned int point_id_;
        
    bool initialized_ = false;
    double target_x_ = 0.0;
    double target_y_ = 0.0;
    double rotation_accumulator_ = 0.0;

    bool initializeTarget(StateSimulation& sim);
    void applyAccumulatedRotation(StateSimulation& sim);
    void executeRotationLogic(StateSimulation& sim, double dt, short rotationNeeded);
    void executeMovementLogic(StateSimulation& sim, double dt, double distanceToTarget);

    double calculateDistanceToTarget(const StateSimulation& sim) const;
    bool hasArrivedAtTarget(StateSimulation& sim, double currentDistance) const;
    bool isAlignedWithTarget(short rotationNeeded, double distanceToTarget) const;
};
