#include "Engine.h"
#include "StateSimulation.h"
#include "Visualizer.h"

#include <chrono>
#include <iostream>

using namespace std::chrono;

namespace {
    constexpr double FIXED_TIMESTEP_SECONDS = 0.02;
    constexpr int TARGET_VISUALIZATION_FPS = 60;
    constexpr double MAX_FRAME_TIME_SECONDS = 0.25;
    constexpr int CPU_YIELD_SLEEP_MS = 1;
}

Engine::Engine(StateSimulation& simulation, Visualizer& visualization)
    : simulation_(simulation)
    , visualization_(visualization)
    , running_(false)
    , speed_multiplier_(1.0)
    , fixed_timestep_(FIXED_TIMESTEP_SECONDS)
{
    user_simulation_callback_ = defaultSimulationLogic;
}

Engine::~Engine() {
    stop();
}

void Engine::start() {
    if (running_) {
        return;
    }
    running_ = true;

    // Start threads
    simulation_thread_ = std::thread(&Engine::simulationLoop, this);
    visualization_thread_ = std::thread(&Engine::visualizationLoop, this);
}

void Engine::stop() {
    if (!running_) {
        return;
    }
    running_ = false;

    // Join threads if they are joinable
    if (simulation_thread_.joinable()) {
        simulation_thread_.join();
    }
    if (visualization_thread_.joinable()) {
        visualization_thread_.join();
    }
}

bool Engine::isRunning() const {
    return running_.load();
}

void Engine::setSimulationSpeed(double multiplier) {
    if (multiplier > 0) {
        speed_multiplier_ = multiplier;
    }
}

void Engine::setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    user_simulation_callback_ = callback;
}

void Engine::defaultSimulationLogic(StateSimulation& simulation, double dt) {
    simulation.simulateMovement(1.0 * dt);
}

double Engine::getSimulationTime() const {
    return static_cast<double>(simulation_.getTime());
}

void Engine::simulationLoop() {
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
            updateSimulation(fixed_timestep_);
            accumulator -= fixed_timestep_;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(CPU_YIELD_SLEEP_MS));
    }
}

void Engine::updateSimulation(double dt) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    if (user_simulation_callback_) {
        user_simulation_callback_(simulation_, dt);
    }
}

void Engine::visualizationLoop() {
    const double target_frame_time = 1.0 / TARGET_VISUALIZATION_FPS;
    
    while (running_) {
        auto start_time = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(state_mutex_);
            visualization_.triggerRepaint();
        }

        auto end_time = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        
        if (elapsed.count() < target_frame_time) {
            std::this_thread::sleep_for(std::chrono::duration<double>(target_frame_time - elapsed.count()));
        }
    }
}

double Engine::calculateInterpolationAlpha(double accumulator, double dt) const {
    return accumulator / dt;
}
