/* 
    Author: Maciej Cieslik
    
    Handles simulation state. StateSimulation is responsible for changing state of the world 
    by manipulating the mover and the lawn. Calculates simulation time.

*/

#pragma once
#include "Lawn.h"
#include "Logger.h"
#include "Mover.h"

class StateSimulation {
private:
    Lawn& lawn_;
    Mover& mover_;
    Logger& logger_;
    u_int64_t time_;

    double countDistanceToBorder(const double& distance) const;
    std::pair<double, double> countBorderPoint() const;
    void calculateMovementTime(const double& distance); 
    void calculateRotationTime(const short& angle);

public:
    StateSimulation(Lawn& lawn, Mover& mover, Logger& logger);
    StateSimulation(const StateSimulation&) = delete;
    StateSimulation& operator=(const StateSimulation&) = delete;
    bool operator==(const StateSimulation& other) const;
    bool operator!=(const StateSimulation& other) const;

    const Lawn& getLawn() const;
    const Mover& getMover() const;
    const Logger& getLogger() const;
    const u_int64_t& getTime() const;

    void simulateMovement(const double& distance);
    void simulateRotation(const short& angle);
};
