/*
    Author: Hanna Biegacz

    Command to rotate the mower by a specified angle (degrees).
    Implements ICommand interface.
    Executes over multiple frames at a controlled rotation speed until complete.
*/

#pragma once
#include "ICommand.h"

class RotateCommand : public ICommand {
public:
    explicit RotateCommand(short angle);
    bool execute(StateSimulation& sim, double dt) override;

    RotateCommand(const RotateCommand&) = delete;
    RotateCommand& operator=(const RotateCommand&) = delete;
private:
    short angle_left_;
    double rotation_accumulator_ = 0.0;

    double calculateRotationStepForFrame(double dt) const;
    void updateInternalRotationState(double step);
    void applyAccumulatedRotationToSimulation(StateSimulation& sim);
    bool isRotationFinished() const;
};
