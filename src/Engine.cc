/*
    Author: Hanna Biegacz
    TODO: description
*/

#include "Engine.h"
#include "StateSimulation.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono;

namespace {
    constexpr double FIXED_TIMESTEP_SECONDS = 0.02;
    constexpr int TARGET_VISUALIZATION_FPS = 60;
    constexpr double MAX_FRAME_TIME_SECONDS = 0.25;
    constexpr int CPU_YIELD_SLEEP_MS = 1;
}

Engine::Engine(StateSimulation& simulation)
    : simulation_(simulation)
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

    simulation_thread_ = std::thread(&Engine::runSimulation, this);
}

void Engine::stop() {
    if (!running_) {
        return;
    }
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
}

double Engine::getSpeedMultiplier() const {
    return speed_multiplier_.load();
}

void Engine::setUserSimulationLogic(std::function<void(StateSimulation&, double)> callback) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    user_simulation_callback_ = callback;
}

void Engine::defaultSimulationLogic(StateSimulation& simulation, double dt) {
    simulation.simulateMovement(simulation.getMover().getSpeed() * dt);
}

double Engine::getSimulationTime() const {
    return static_cast<double>(simulation_.getTime());
}

RenderContext& Engine::getRenderContext() {
    return render_context_;
}

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
            updateSimulation(fixed_timestep_);
            accumulator -= fixed_timestep_;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(CPU_YIELD_SLEEP_MS));
    }
}

void Engine::updateSimulation(double dt) {
    // {
    std::lock_guard<std::mutex> lock(state_mutex_);
    if (user_simulation_callback_) {
        user_simulation_callback_(simulation_, dt);
    }
// }
    render_context_.addSnapshot(simulation_.buildSnapshot());
}

