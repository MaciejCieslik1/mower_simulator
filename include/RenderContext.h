/*
    Author: Hanna Biegacz
    Manages simulation state interpolation for smooth rendering.
*/

#pragma once

#include <mutex>
#include <cmath>
#include <vector>
#include "Point.h"

struct Snapshot {
    double x_ = 0;
    double y_ = 0;
    double angle_ = 0;
    double simulation_time_ = 0;

    unsigned int lawn_width_ = 0;
    unsigned int lawn_length_ = 0;
    std::vector<std::vector<bool>> fields_;

    double mower_width_ = 0;
    double mower_length_ = 0;
    double blade_diameter_ = 0;

    std::vector<Point> points_;
};

class RenderContext {
public:
    void addSnapshot(const Snapshot& snapshot) {
        std::lock_guard<std::mutex> lock(mutex_);
        previous_snapshot_ = current_snapshot_;
        current_snapshot_ = snapshot;
    }

    Snapshot getInterpolatedState(double render_time) const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (current_snapshot_.simulation_time_ <= previous_snapshot_.simulation_time_) {
            return current_snapshot_;
        }

        double alpha = (render_time - previous_snapshot_.simulation_time_) / 
                       (current_snapshot_.simulation_time_ - previous_snapshot_.simulation_time_);
        
        if (alpha < 0.0) alpha = 0.0;
        if (alpha > 1.0) alpha = 1.0;

        Snapshot result = current_snapshot_;
        result.x_ = interpolate(previous_snapshot_.x_, current_snapshot_.x_, alpha);
        result.y_ = interpolate(previous_snapshot_.y_, current_snapshot_.y_, alpha);
        result.angle_ = interpolateAngle(previous_snapshot_.angle_, current_snapshot_.angle_, alpha);
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

    Snapshot previous_snapshot_;
    Snapshot current_snapshot_;
    mutable std::mutex mutex_;
};
