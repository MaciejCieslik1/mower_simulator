/* 
    Author: Hanna Biegacz
    Implementation of RenderTimeController.
*/

#include "RenderTimeController.h"
using namespace std;

RenderTimeController::RenderTimeController(const StateInterpolator& interpolator)
    : state_interpolator_(interpolator) {
}

// Updates the render time each frame. On first run, initializes the time slightly
// behind the simulation. Then smoothly advances time and corrects any drift to keep
// rendering synchronized with the simulation.
void RenderTimeController::update(double dt_ms) {
    double current_sim_time = state_interpolator_.getSimulationTime();
    double current_speed = state_interpolator_.getSpeedMultiplier();

    if (smoothed_render_time_ == 0.0 && current_sim_time > 0.0) {
        smoothed_render_time_ = calculateIdealRenderTime(current_sim_time, current_speed);
        return;
    }

    advanceRenderTimeByDelta(dt_ms, current_speed);
    syncWithSimulationClock(current_sim_time, current_speed);
}

double RenderTimeController::getSmoothedTime() const {
    return smoothed_render_time_;
}

// Calculates where the render time should be. The render time is intentionally kept
// behind the simulation time by a buffer delay. This gives the interpolator enough
// snapshot history to blend between, ensuring smooth animation even if frames arrive
// irregularly. Higher speeds need bigger buffers.
double RenderTimeController::calculateIdealRenderTime(double actual_sim_time, double speed_multiplier) const {
    double dynamic_delay = BASE_BUFFER_DELAY_MS * max(1.0, speed_multiplier);
    return max(0.0, actual_sim_time - dynamic_delay);
}

void RenderTimeController::advanceRenderTimeByDelta(double dt_ms, double speed_multiplier) {
    smoothed_render_time_ += (dt_ms * speed_multiplier);
}

// Checks if render time has drifted too far from where it should be and corrects it.
// Small drifts are fixed gradually (smooth correction), large drifts are fixed
// immediately (hard reset). This happens when simulation speed changes or if there's
// a performance hiccup. Also prevents render time from going ahead of simulation time.
void RenderTimeController::syncWithSimulationClock(double actual_sim_time, double speed_multiplier) {
    double ideal_time = calculateIdealRenderTime(actual_sim_time, speed_multiplier);
    double time_drift = abs(ideal_time - smoothed_render_time_);
    
    double max_allowed_drift = MAX_TIME_DRIFT_MS * max(1.0, speed_multiplier);

    if (time_drift > max_allowed_drift) {
        forceImmediateTimeReset(ideal_time);
    } else {
        applyGradualTimeCorrection(ideal_time);
    }

    if (smoothed_render_time_ > actual_sim_time) {
        smoothed_render_time_ = actual_sim_time;
    }
}

// Gently nudges render time toward the ideal time by a small fraction each frame.
// This creates smooth corrections that are invisible to the user.
void RenderTimeController::applyGradualTimeCorrection(double ideal_time) {
    double correction = (ideal_time - smoothed_render_time_) * DRIFT_CORRECTION_FACTOR;
    smoothed_render_time_ += correction;
}

// Instantly jumps render time to the ideal value. Used only when drift 
// is too large to correct smoothly.
void RenderTimeController::forceImmediateTimeReset(double ideal_time) {
    smoothed_render_time_ = ideal_time;
}
