/*
    Author: Hanna Biegacz

    Command to rotate the mower towards a specific point.
    Implements ICommand interface.
    Executes over multiple frames at a controlled rotation speed until alignment with target point.
*/

#pragma once
#include "ICommand.h"

class RotateTowardsPointCommand : public ICommand {
public:
    explicit RotateTowardsPointCommand(unsigned int pointId);
    bool execute(StateSimulation& sim, double dt) override;

    RotateTowardsPointCommand(const RotateTowardsPointCommand&) = delete;
    RotateTowardsPointCommand& operator=(const RotateTowardsPointCommand&) = delete;
private:
    unsigned int point_id_;
    bool initialized_ = false;
    double target_x_ = 0.0;
    double target_y_ = 0.0;
    double rotation_accumulator_ = 0.0;

    bool initializeTarget(StateSimulation& sim);
    void executeRotationLogic(StateSimulation& sim, double dt, short rotationNeeded);
    void applyAccumulatedRotation(StateSimulation& sim);
    bool isAlignedWithTarget(short rotationNeeded) const;
};
