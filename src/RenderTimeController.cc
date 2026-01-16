/* 
    Author: Hanna Biegacz
    Implementation of RenderTimeController.
*/

#include "RenderTimeController.h"

RenderTimeController::RenderTimeController(const StateInterpolator& interpolator)
    : state_interpolator_(interpolator) {
}

void RenderTimeController::update(double dt_ms) {
    double current_sim_time = state_interpolator_.getSimulationTime();
    double current_speed = state_interpolator_.getSpeedMultiplier();

    if (smoothed_render_time_ == 0.0 && current_sim_time > 0.0) {
        smoothed_render_time_ = calculateIdealRenderTime(current_sim_time, current_speed);
        return;
    }

    advanceRenderTime(dt_ms, current_speed);
    syncWithSimulationClock(current_sim_time, current_speed);
}

double RenderTimeController::getSmoothedTime() const {
    return smoothed_render_time_;
}

void RenderTimeController::reset() {
    smoothed_render_time_ = 0.0;
}

double RenderTimeController::calculateIdealRenderTime(double actual_sim_time, double speed_multiplier) const {
    double dynamic_delay = BASE_BUFFER_DELAY_MS * std::max(1.0, speed_multiplier);
    return std::max(0.0, actual_sim_time - dynamic_delay);
}

void RenderTimeController::advanceRenderTime(double dt_ms, double speed_multiplier) {
    smoothed_render_time_ += (dt_ms * speed_multiplier);
}

void RenderTimeController::syncWithSimulationClock(double actual_sim_time, double speed_multiplier) {
    double ideal_time = calculateIdealRenderTime(actual_sim_time, speed_multiplier);
    double time_drift = std::abs(ideal_time - smoothed_render_time_);
    
    double max_allowed_drift = MAX_TIME_DRIFT_MS * std::max(1.0, speed_multiplier);

    if (time_drift > max_allowed_drift) {
        performHardTimeReset(ideal_time);
    } else {
        applySoftTimeCorrection(ideal_time);
    }

    if (smoothed_render_time_ > actual_sim_time) {
        smoothed_render_time_ = actual_sim_time;
    }
}

void RenderTimeController::applySoftTimeCorrection(double ideal_time) {
    double correction = (ideal_time - smoothed_render_time_) * DRIFT_CORRECTION_FACTOR;
    smoothed_render_time_ += correction;
}

void RenderTimeController::performHardTimeReset(double ideal_time) {
    smoothed_render_time_ = ideal_time;
}
