/*
    Author: Hanna Biegacz

    Implementation of a user command.
*/

#include "commands/GetDistanceToPointCommand.h"
#include <cmath>
#include <string>

GetDistanceToPointCommand::GetDistanceToPointCommand(unsigned int pointId, double& outDistance)
    : point_id_(pointId), out_distance_(outDistance) {}

// Calculates the distance from the mower to a specific point and stores it
// in the output reference variable (completes in one frame).
bool GetDistanceToPointCommand::execute(StateSimulation& sim, double dt) {
    auto coords = sim.getPointCoordinates(point_id_);
    if (!coords) {
        logPointNotFoundError(sim);
        return true; 
    }

    double distance = calculateDistanceFromMowerToPoint(sim, coords->first, coords->second);
    out_distance_ = distance;
    logDistanceResult(sim, distance);

    return true;
}

void GetDistanceToPointCommand::logPointNotFoundError(StateSimulation& sim) const {
    std::string msg = "Error: Point " + std::to_string(point_id_) + " not found for GetDistanceToPoint.";
    sim.getFileLogger().saveMessage(msg);
}

double GetDistanceToPointCommand::calculateDistanceFromMowerToPoint(const StateSimulation& sim, double target_x, double target_y) const {
    double current_x = sim.getMower().getX();
    double current_y = sim.getMower().getY();
    double dx = target_x - current_x;
    double dy = target_y - current_y;
    return std::sqrt(dx*dx + dy*dy);
}

void GetDistanceToPointCommand::logDistanceResult(StateSimulation& sim, double distance) const {
    std::string msg = "Distance to point " + std::to_string(point_id_) + ": " + std::to_string(distance);
    sim.getFileLogger().saveMessage(msg);
}
