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
    Engine(StateSimulation& simulation, 
           std::function<void(StateSimulation&, double)> user_logic = nullptr,
           std::function<void(const std::string&)> error_callback = nullptr);
    ~Engine();

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void start();
    void stop();
    bool isRunning() const;

    void setSimulationSpeed(double multiplier);
    double getSpeedMultiplier() const;
    double getSimulationTime() const; 
    StateInterpolator& getStateInterpolator();

    void setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback);
    void setOnErrorCallback(std::function<void(const std::string&)> callback);
    static void defaultSimulationLogic(StateSimulation& simulation, double dt);

private:
    void runSimulation();
    void updateSimulation(double dt);
    void processLogs(); 

    StateSimulation& simulation_;
    StateInterpolator state_interpolator_;
    std::thread simulation_thread_;
    std::mutex state_mutex_; 
    std::atomic<bool> running_;
    std::atomic<double> speed_multiplier_;
    const double fixed_timestep_; 

    std::function<void(StateSimulation&, double)> user_simulation_callback_;
    std::function<void(const std::string&)> error_callback_;
};
