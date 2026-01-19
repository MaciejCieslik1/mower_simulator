/* 
    Author: Maciej Cieslik, Hanna Biegacz
    
    Handles simulation state. StateSimulation is responsible for changing state of the world 
    by manipulating the mower and the lawn. Calculates simulation time and creates lightweight 
    snapshots for the interpolator (to minimize locking time and build a history buffer for smooth animation).

*/

#pragma once
#include <optional>
#include "Point.h"
#include "Lawn.h"
#include "Logger.h"
#include "Mower.h"
#include "FileLogger.h"
#include "StateInterpolator.h"

class StateSimulation {
private:
    Lawn& lawn_;
    Mower& mower_;
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
    StateSimulation(Lawn& lawn, Mower& mower, Logger& logger, FileLogger& file_logger);
    StateSimulation(const StateSimulation&) = delete;
    StateSimulation& operator=(const StateSimulation&) = delete;
    bool operator==(const StateSimulation& other) const;
    bool operator!=(const StateSimulation& other) const;

    const Lawn& getLawn() const;
    const Mower& getMower() const;
    Logger& getLogger();
    const Logger& getLogger() const;
    const u_int64_t& getTime() const;
    const std::vector<Point>& getPoints() const;
    const unsigned int& getNextPointId() const;
    StaticSimulationData getStaticData() const;
    const FileLogger& getFileLogger() const;
    void logArrivalAtPoint(unsigned int pointId);
    SimulationSnapshot buildSimulationSnapshot() const;
    std::optional<std::pair<double, double>> getPointCoordinates(unsigned int pointId);
    std::pair<short, double> calculateNavigationVector(double targetX, double targetY) const; 

    void simulateMovement(const double& distance);
    void simulateRotation(const short& angle);
    void simulateMowingOptionOn();
    void simulateMowingOptionOff();
    void simulateAddPoint(const double& x, const double& y);
    void simulateDeletePoint(const unsigned int& pointIndex);
    void simulateMovementToPoint(const unsigned int& pointIndex);

};
