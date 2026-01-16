/*
    Author: Hanna Biegacz
    Manages simulation state interpolation for smooth rendering.
*/

#pragma once
#include <atomic>
#include <mutex>
#include <cmath>
#include <deque>
#include <algorithm>
#include "Point.h"
#include "SimulationSnapshot.h"


class RenderContext {
    // TODO: improve name to StateInterpolator
    // TODO: better interpolation! przy większej prędkości kosiarki, niestety skacze
    // TODO: Separate .cc file

    std::atomic<double> latest_sim_time_{0.0};
    std::atomic<double> current_speed_multiplier_{1.0}; //why atomic? why 1.0?
public:
    void addSimulationSnapshot(const SimulationSnapshot& sim_snapshot) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!sim_snapshot_buffer_.empty() && sim_snapshot.simulation_time_ <= sim_snapshot_buffer_.back().simulation_time_) {
            return;
        }
        sim_snapshot_buffer_.push_back(sim_snapshot);
        
        while (sim_snapshot_buffer_.size() > MAX_BUFFER_SIZE) {
            sim_snapshot_buffer_.pop_front();
        }
    }

    SimulationSnapshot getInterpolatedState(double render_time) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (sim_snapshot_buffer_.empty()) {
            return SimulationSnapshot();
        }

        if (sim_snapshot_buffer_.size() == 1 || render_time <= sim_snapshot_buffer_.front().simulation_time_) {
            return sim_snapshot_buffer_.front();
        }

        if (render_time >= sim_snapshot_buffer_.back().simulation_time_) {
            return sim_snapshot_buffer_.back();
        }

        auto it = std::lower_bound(sim_snapshot_buffer_.begin(), sim_snapshot_buffer_.end(), render_time,
            [](const SimulationSnapshot& s, double t) {
                return s.simulation_time_ < t;
            });

        if (it == sim_snapshot_buffer_.begin()) return sim_snapshot_buffer_.front();
        
        const SimulationSnapshot& s2 = *it;
        const SimulationSnapshot& s1 = *std::prev(it);

        double alpha = (render_time - s1.simulation_time_) / 
                       (s2.simulation_time_ - s1.simulation_time_);
        
        alpha = std::clamp(alpha, 0.0, 1.0);

        SimulationSnapshot result = s2;
        result.x_ = interpolate(s1.x_, s2.x_, alpha);
        result.y_ = interpolate(s1.y_, s2.y_, alpha);
        result.angle_ = interpolateAngle(s1.angle_, s2.angle_, alpha);
        result.simulation_time_ = render_time;
        return result;
    }

    void setSimulationMetadata(double time, double speed) {
        latest_sim_time_.store(time);
        current_speed_multiplier_.store(speed);
    }

    double getSimulationTime() const { return latest_sim_time_.load(); }
    double getSpeedMultiplier() const { return current_speed_multiplier_.load(); }

private:

    static double interpolate(double a, double b, double alpha) { 
        return a + (b - a) * alpha;
    }

    static double interpolateAngle(double start_angle, double end_angle, double alpha) {
        double diff = end_angle - start_angle;
        while (diff < -180.0) diff += 360.0;
        while (diff > 180.0) diff -= 360.0;
        return start_angle + diff * alpha;
    }

    std::deque<SimulationSnapshot> sim_snapshot_buffer_;
    static constexpr size_t MAX_BUFFER_SIZE = 50;
    mutable std::mutex mutex_;
};
