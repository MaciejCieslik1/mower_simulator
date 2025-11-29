#include "simulation/SimulationEngine.h"
#include "../../include/Lawn.h"
#include <iostream>

using namespace std;

SimulationEngine::SimulationEngine(Lawn* lawn)
    : lawn_(lawn), is_running_(false), speed_multiplier_(1.0), current_row_(0), current_col_(0) {
    
    if (!lawn_) {
        cerr << "[SimulationEngine] ERROR: Lawn pointer is null!" << endl;
        throw invalid_argument("Lawn pointer cannot be null");
    }    
}

SimulationEngine::~SimulationEngine() {
    if (is_running_) {
        stop();
    }
}

void SimulationEngine::start() {
    if (is_running_) {
        return;
    }

    is_running_ = true;
    simulation_thread_ = thread(&SimulationEngine::simulationLoop, this);
}

void SimulationEngine::stop() {
    if (!is_running_) {
        return;
    }

    is_running_ = false;
    
    if (simulation_thread_.joinable()) {
        simulation_thread_.join();
    }
}

void SimulationEngine::setSimulationSpeed(const double speed_multiplier) {
    if (speed_multiplier <= 0.0) {
        return;
    }
    
    speed_multiplier_ = speed_multiplier;
}

double SimulationEngine::getSimulationSpeed() const {
    return speed_multiplier_.load();
}

mutex& SimulationEngine::getLawnMutex() {
    return lawn_mutex_;
}

bool SimulationEngine::isRunning() const {
    return is_running_.load();
}

void SimulationEngine::simulationLoop() {
    using Clock = chrono::steady_clock;
    using Ms = chrono::milliseconds;

    auto next_tick = Clock::now();
    
    auto fields = lawn_->getFields();
    const unsigned int total_rows = fields.size();
    const unsigned int total_cols = fields[0].size();
    
    while (is_running_) {
        const double speed = speed_multiplier_.load();
        const int adjusted_timestep = static_cast<int>(TIMESTEP_MS / speed);
        
        next_tick += Ms(adjusted_timestep);
        
        executeSimulationStep();
        
        this_thread::sleep_until(next_tick);
    }
}

void SimulationEngine::executeSimulationStep() {
    lock_guard<mutex> lock(lawn_mutex_);
    
    auto fields = lawn_->getFields();
    const unsigned int total_rows = fields.size();
    const unsigned int total_cols = fields[0].size();
    
    if (current_row_ >= total_rows) {
        current_row_ = 0;
        current_col_ = 0;
        return;
    }
    
    lawn_->cutGrassOnField({current_col_, current_row_});
    
    ++current_col_;
    if (current_col_ >= total_cols) {
        current_col_ = 0;
        ++current_row_;
    }
}
