/*
    Author: Hanna Biegacz
    This class is the main coordinator of the simulation.
    It runs the simulation loop in a separate thread (fixed timestep) and manages time speed. 
    It also handles synchronization (mutexes) to safely connect the 
    logic update with the visualization.
*/

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include "StateInterpolator.h"

class StateSimulation;

class Engine {
public:
    Engine(StateSimulation& simulation);
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void start();
    void stop();
    bool isRunning() const;

    void setSimulationSpeed(double multiplier);
    double getSpeedMultiplier() const;
    void setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback);

    static void defaultSimulationLogic(StateSimulation& simulation, double dt);

    double getSimulationTime() const; 
    StateInterpolator& getStateInterpolator();

private:
    void runSimulation();
    void updateSimulation(double dt);

    StateSimulation& simulation_;

    StateInterpolator state_interpolator_;

    std::thread simulation_thread_;
    std::mutex state_mutex_; 
    std::atomic<bool> running_;

    std::atomic<double> speed_multiplier_;
    const double fixed_timestep_; 

    std::function<void(StateSimulation&, double)> user_simulation_callback_;
};
