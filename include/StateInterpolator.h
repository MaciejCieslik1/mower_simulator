/*
    Author: Hanna Biegacz
    Manages simulation state interpolation for smooth rendering.
*/

#pragma once

#include <atomic>
#include <mutex>
#include <deque>
#include "SimulationSnapshot.h"

struct StaticSimulationData {
    unsigned int lawn_width_ = 0;
    unsigned int lawn_length_ = 0;
    
    double width_cm = 0.0;
    double length_cm = 0.0;
    double blade_diameter_cm = 0.0;
};

class StateInterpolator {
public:
    void addSimulationSnapshot( const SimulationSnapshot& sim_snapshot );
    SimulationSnapshot getInterpolatedState( double render_time ) const;

    void setStaticSimulationData(const StaticSimulationData& data);
    const StaticSimulationData& getStaticSimulationData() const;
    double getSimulationTime() const;
    double getSpeedMultiplier() const;
    void setSimulationSpeedMultiplier(double speed_multiplier);
private:
    StaticSimulationData static_simulation_data;
    std::deque<SimulationSnapshot> sim_snapshot_buffer_;
    std::atomic<double> current_speed_multiplier_{1.0};
    mutable std::mutex mutex_;
    
    static const size_t MAX_BUFFER_SIZE = 50;

    void storeSnapshot( const SimulationSnapshot& snapshot );
    SimulationSnapshot blendSnapshots( const SimulationSnapshot& start, const SimulationSnapshot& end, double alpha, double render_time ) const;
    void enforceBufferSizeLimit();
    bool isSnapshotOutdatedOrDuplicate( const SimulationSnapshot& snapshot ) const;
    bool shouldReturnEarliestSnapshot( double render_time ) const;
    bool shouldReturnLatestSnapshot( double render_time ) const;
    bool tryUpdateExistingSnapshot(const SimulationSnapshot& snapshot);
    std::deque<SimulationSnapshot>::const_iterator findFirstSnapshotAfter( double time ) const;

    SimulationSnapshot computeInterpolatedSnapshot( double render_time ) const;
    double calculateInterpolationAlpha( const SimulationSnapshot& before, const SimulationSnapshot& after, double render_time ) const;
    static double interpolate( double a, double b, double alpha );
    static double interpolateAngle( double start_angle, double end_angle, double alpha );
};
