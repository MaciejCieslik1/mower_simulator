/* 
    Author: Hanna Biegacz
    
    This class controls the animation time to ensure smooth movement.
    It calculates exactly which moment of the simulation should be shown right now.
    It handles synchronization between real time and simulation time.
*/

#pragma once

#include "StateInterpolator.h"
#include <cmath>
#include <algorithm>

class RenderTimeController {
public:
    explicit RenderTimeController(const StateInterpolator& interpolator);
    
    void update(double dt_ms);
    double getSmoothedTime() const;
    void reset();

private:
    static constexpr double BASE_BUFFER_DELAY_MS = 200.0;  
    static constexpr double MAX_TIME_DRIFT_MS = 2000.0;    
    static constexpr double DRIFT_CORRECTION_FACTOR = 0.1; 

    const StateInterpolator& state_interpolator_;
    double smoothed_render_time_ = 0.0;

    double calculateIdealRenderTime(double actual_sim_time, double speed_multiplier) const;
    void advanceRenderTime(double dt_ms, double speed_multiplier);
    void syncWithSimulationClock(double actual_sim_time, double speed_multiplier);
    void applySoftTimeCorrection(double ideal_time);
    void performHardTimeReset(double ideal_time);
};
