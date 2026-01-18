/*
    Author: Hanna Biegacz

    StateInterpolator makes the mower move smoothly in the visualization. 
    It saves a list of recent simulation snapshots of the simulation 
    and calculates the "in-between" positions for the animation. 
    This prevents the mower from jumping and teleporting.
*/

#pragma once

#include <atomic>
#include <mutex>
#include <deque>
#include "SimulationSnapshot.h"

struct StaticSimulationData {
    unsigned int lawn_width_ = 0;
    unsigned int lawn_length_ = 0;
    
    double width_cm_ = 0.0;
    double length_cm = 0.0;
    double blade_diameter_cm = 0.0;
};

class StateInterpolator {
public:
    StateInterpolator() = default;
    StateInterpolator(const StateInterpolator&) = delete;
    StateInterpolator& operator=(const StateInterpolator&) = delete;

    void addSimulationSnapshot( const SimulationSnapshot& sim_snapshot );
    SimulationSnapshot getInterpolatedState( double render_time ) const;

    double getSimulationTime() const;
    double getSpeedMultiplier() const;
    const StaticSimulationData& getStaticSimulationData() const;

    void setSimulationSpeedMultiplier(double speed_multiplier);
    void setStaticSimulationData(const StaticSimulationData& data);
private:
    StaticSimulationData static_simulation_data;
    std::deque<SimulationSnapshot> sim_snapshot_buffer_;
    std::atomic<double> current_speed_multiplier_{1.0};
    mutable std::mutex mutex_;
    
    static const size_t MAX_BUFFER_SIZE = 50;

    void storeSnapshot( const SimulationSnapshot& snapshot );
    void enforceBufferSizeLimit();
    bool isSnapshotOutdated( const SimulationSnapshot& snapshot ) const;
    bool tryUpdateExistingSnapshot(const SimulationSnapshot& snapshot);


    SimulationSnapshot computeInterpolatedSnapshot( double render_time ) const;
    SimulationSnapshot blendSnapshots( const SimulationSnapshot& start, const SimulationSnapshot& end, double alpha, double render_time ) const;
    double calculateInterpolationAlpha( const SimulationSnapshot& before, const SimulationSnapshot& after, double render_time ) const;
    static double interpolate( double a, double b, double alpha );
    static double interpolateAngle( double start_angle, double end_angle, double alpha );

    bool shouldReturnEarliestSnapshot( double render_time ) const;
    bool shouldReturnLatestSnapshot( double render_time ) const;
    std::deque<SimulationSnapshot>::const_iterator findFirstSnapshotAfter( double time ) const;
};
