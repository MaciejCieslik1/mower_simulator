/*
    Author: Hanna Biegacz
    Implementation of the simulation Engine
*/

#include <chrono>
#include <iostream>
#include <thread>
#include "Engine.h"
#include "StateSimulation.h"
#include "Exceptions.h"

using namespace std::chrono;

namespace {
    constexpr double FIXED_TIMESTEP_SECONDS = 0.02;
    constexpr int TARGET_VISUALIZATION_FPS = 60;
    constexpr double MAX_FRAME_TIME_SECONDS = 0.25;
    constexpr int CPU_YIELD_SLEEP_MS = 1;
}

Engine::Engine(StateSimulation& simulation, 
               std::function<void(StateSimulation&, double)> user_logic,
               std::function<void(const std::string&)> error_callback)
    : simulation_(simulation)
    , running_(false)
    , speed_multiplier_(1.0)
    , fixed_timestep_(FIXED_TIMESTEP_SECONDS)
    , user_simulation_callback_(user_logic ? user_logic : defaultSimulationLogic)
    , error_callback_(error_callback)
{
    state_interpolator_.setStaticSimulationData(simulation_.getStaticData());
}

Engine::~Engine() {
    stop();
}

// Launches the simulation in a separate thread so it runs independently
// from the visualization. Does nothing if already running.
void Engine::start() {
    if (running_) {
        return;
    }
    running_ = true;

    simulation_thread_ = std::thread(&Engine::runSimulation, this);
}

// Safely shuts down the simulation thread. Waits for the thread
// to finish before returning to prevent crashes.
void Engine::stop() {
    running_ = false;

    if (simulation_thread_.joinable()) {
        simulation_thread_.join();
    }
}

bool Engine::isRunning() const {
    return running_.load();
}

void Engine::setSimulationSpeed(double multiplier) {
    if (multiplier > 0) {
        speed_multiplier_ = multiplier;
    }
    state_interpolator_.setSimulationSpeedMultiplier(multiplier);
}

double Engine::getSpeedMultiplier() const {
    return speed_multiplier_.load();
}

// Allows you to replace the default simulation behavior with custom logic.
// The callback function will be called every simulation step.
void Engine::setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    user_simulation_callback_ = callback;
}

void Engine::setOnErrorCallback(std::function<void(const std::string&)> callback) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    error_callback_ = callback;
}

void Engine::defaultSimulationLogic(StateSimulation& simulation, double dt) {
    // by default the mower is doing nothing
}

double Engine::getSimulationTime() const {
    return static_cast<double>(simulation_.getTime());
}

StateInterpolator& Engine::getStateInterpolator() {
    return state_interpolator_;
}

// Main simulation loop running in a separate thread. Uses "fixed timestep" which means
// the simulation always advances by exactly 0.02 seconds per step, regardless of how
// fast or slow the computer is. This keeps physics consistent and predictable.
// The "accumulator" collects real time that has passed and converts it into fixed steps.
void Engine::runSimulation() {
    using Clock = std::chrono::steady_clock;
    auto previous_time = Clock::now();
    double accumulator = 0.0;

    while (running_) {
        auto current_time = Clock::now();
        duration<double> frame_time = current_time - previous_time;
        previous_time = current_time;

        if (frame_time.count() > MAX_FRAME_TIME_SECONDS) {
            frame_time = duration<double>(MAX_FRAME_TIME_SECONDS);
        }

        accumulator += frame_time.count() * speed_multiplier_.load();

        while (accumulator >= fixed_timestep_) {
            try {
                updateSimulation(fixed_timestep_);
            } catch (const MoveOutsideLawnError& e) {
                std::cerr << "[Engine] Simulation stopped: " << e.what() << std::endl;
                running_ = false;
                if (error_callback_) {
                    error_callback_(e.what());
                }
                break;
            }
            accumulator -= fixed_timestep_;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(CPU_YIELD_SLEEP_MS));
    }
}

// Executes one simulation step: runs user logic, saves logs, and creates
// a snapshot for smooth rendering. Thread-safe with mutex lock.
void Engine::updateSimulation(double dt) {
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        if (user_simulation_callback_) {
            user_simulation_callback_(simulation_, dt);
        }
        processLogs();
    }
    state_interpolator_.addSimulationSnapshot(simulation_.buildSimulationSnapshot());
    state_interpolator_.setSimulationSpeedMultiplier(speed_multiplier_.load());
}

void Engine::processLogs() {
    Logger& logger = simulation_.getLogger();
    std::queue<Log> logsQueue = logger.getLogs();
    if (logsQueue.empty()) {
        return;
    }

    const FileLogger& fileLogger = simulation_.getFileLogger();
    
    while (!logsQueue.empty()) {
        fileLogger.saveLog(logsQueue.front());
        logsQueue.pop();
    }

    logger.clear();
}
