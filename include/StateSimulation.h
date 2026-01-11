/* 
    Author: Maciej Cieslik
    
    Handles simulation state. StateSimulation is responsible for changing state of the world 
    by manipulating the mover and the lawn. Calculates simulation time.

*/

#pragma once
#include "Point.h"
#include "Lawn.h"
#include "Logger.h"
#include "Mover.h"
#include "FileLogger.h"

class StateSimulation {
private:
    Lawn& lawn_;
    Mover& mover_;
    Logger& logger_;
    u_int64_t time_;
    std::vector<Point> points_;
    unsigned int next_point_id_;
    FileLogger file_logger_;

    double countDistanceToBorder(const double& distance) const;
    std::pair<double, double> countBorderPoint() const;
    void calculateMovementTime(const double& distance);  
    void calculateRotationTime(const short& angle);
    void moveToPointAttempt(const double& x, const double& y);
    std::pair<short, double> calculateAngleAndDistance(const double& x, const double& y) const;
    double calculateRotationNoDx(const double& dy) const;
    double calculateRotationDx(const double& dy, const double& dx) const;

public:
    StateSimulation(Lawn& lawn, Mover& mover, Logger& logger, FileLogger& file_logger);
    StateSimulation(const StateSimulation&) = delete;
    StateSimulation& operator=(const StateSimulation&) = delete;
    bool operator==(const StateSimulation& other) const;
    bool operator!=(const StateSimulation& other) const;

    const Lawn& getLawn() const;
    const Mover& getMover() const;
    const Logger& getLogger() const;
    const u_int64_t& getTime() const;
    const std::vector<Point>& getPoints() const;
    const unsigned int& getNextPointId() const;
    const FileLogger& getFileLogger() const;

    void simulateMovement(const double& distance);
    void simulateRotation(const short& angle);
    void simulateMowingOptionOn();
    void simulateMowingOptionOff();
    void simulateAddPoint(const double& x, const double& y);
    void simulateDeletePoint(const unsigned int& pointIndex);
    void simulateMovementToPoint(const unsigned int& pointIndex);
};
