/*
    Author: Hanna Biegacz

    Command to calculate and retrieve the distance from the mower to a specific point.
    Implements ICommand interface.
*/


#pragma once
#include "ICommand.h"
#include <string>

class GetDistanceToPointCommand : public ICommand {
public:
    GetDistanceToPointCommand(unsigned int pointId, double& outDistance);
    bool execute(StateSimulation& sim, double dt) override;

    GetDistanceToPointCommand(const GetDistanceToPointCommand&) = delete;
    GetDistanceToPointCommand& operator=(const GetDistanceToPointCommand&) = delete;
private:
    unsigned int point_id_;
    double& out_distance_;
    
    void logPointNotFoundError(StateSimulation& sim) const;
    double calculateDistanceFromMowerToPoint(const StateSimulation& sim, double target_x, double target_y) const;
    void logDistanceResult(StateSimulation& sim, double distance) const;
};
