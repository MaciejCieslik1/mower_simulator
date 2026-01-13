/*
    Author: Hanna Biegacz
    Engine class - main coordinator for the simulation.
    
    Responsibilities:
    - Manages two separate threads: simulation and visualization
    - Synchronizes access to StateSimulation using mutex
    - Provides speed control for simulation (time multiplier)
    - Executes user-defined simulation logic callbacks
*/

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class StateSimulation;
class Visualizer;

class Engine {
public:
    Engine(StateSimulation& simulation, Visualizer& visualization);
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void start();
    void stop();
    bool isRunning() const;

    void setSimulationSpeed(double multiplier);
    void setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback);

    static void defaultSimulationLogic(StateSimulation& simulation, double dt);

    double getSimulationTime() const; // Returns simulation time from StateSimulation
    // double getFPS() const; // Optional, can be added later

private:
    void simulationLoop();
    void visualizationLoop();
    void updateSimulation(double dt);
    double calculateInterpolationAlpha(double accumulator, double dt) const;

    StateSimulation& simulation_;
    Visualizer& visualization_;

    std::thread simulation_thread_;
    std::thread visualization_thread_;
    std::mutex state_mutex_; 
    std::atomic<bool> running_;

    std::atomic<double> speed_multiplier_;
    const double fixed_timestep_; 

    std::function<void(StateSimulation&, double)> user_simulation_callback_;
};
