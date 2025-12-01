/* 
    Author: Hanna Biegacz
    
    Runs the lawn mowing simulation in a separate thread.
    Controls how fast the simulation runs and keeps lawn data safe
    when multiple parts of the program need to access it at the same time.
    
    PROTOTYPE STAGE: Right now this class automatically mows grass row by row
    to test the visualization system. In the final version, a Mower object
    will handle the actual mowing logic.
*/

#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class Lawn;

class SimulationEngine {
public:
    explicit SimulationEngine(Lawn* lawn);
    ~SimulationEngine();
    
    SimulationEngine(const SimulationEngine&) = delete;
    SimulationEngine& operator=(const SimulationEngine&) = delete;
    
    void start();
    void stop();
    void setSimulationSpeed(const double speed_multiplier);
    double getSimulationSpeed() const;
    
    std::mutex& getLawnMutex();
    bool isRunning() const;
    
private:
    void simulationLoop();
    void executePrototypeMowingStep(); // TODO: Replace with Mower class 
    
    Lawn* lawn_;
    std::thread simulation_thread_;
    std::atomic<bool> is_running_;
    std::atomic<double> speed_multiplier_;
    std::mutex lawn_mutex_;
    
    // PROTOTYPE STAGE ONLY: Remove these fields when Mower class is implemented
    unsigned int current_row_;   // Temporary: tracks mock mowing position
    unsigned int current_col_;   // Temporary: tracks mock mowing position
    
    static constexpr int TIMESTEP_MS = 10;
};