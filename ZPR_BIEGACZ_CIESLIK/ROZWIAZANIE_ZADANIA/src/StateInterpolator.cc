/*
    Author: Hanna Biegacz
    Implementation of StateInterpolator.
*/

#include <algorithm> 
#include <cmath>     
#include <iterator>  
#include "StateInterpolator.h"

using namespace std;


// Adds a new snapshot to the buffer. Thread-safe with mutex lock.
// If a snapshot with the same timestamp already exists, it updates that one instead.
// (Some actions, like removing point or turning mowing on/off happen instantaneously and do not move the simulation time forward)
// Rejects snapshots that are older than the newest one to keep time moving forward.
void StateInterpolator::addSimulationSnapshot( const SimulationSnapshot& sim_snapshot ){
    lock_guard<mutex> lock( mutex_ );
    
    if (tryUpdateExistingSnapshot(sim_snapshot)) {
        return;
    }

    if( isSnapshotOutdated( sim_snapshot ) ){
        return;
    }
    
    storeSnapshot( sim_snapshot );
    enforceBufferSizeLimit();
}

bool StateInterpolator::tryUpdateExistingSnapshot(const SimulationSnapshot& snapshot) {
    if (sim_snapshot_buffer_.empty()) {
        return false;
    }

    if (snapshot.simulation_time_ == sim_snapshot_buffer_.back().simulation_time_) {
        sim_snapshot_buffer_.back() = snapshot;
        return true;
    }

    return false;
}

// Returns a smoothly interpolated snapshot for the requested render time.
// Interpolation means blending between two snapshots to create in-between positions.
// This is what makes the mower move smoothly instead of jumping between snapshots.
// Thread-safe with mutex lock.
SimulationSnapshot StateInterpolator::getInterpolatedState( double render_time ) const {
    lock_guard<mutex> lock( mutex_ );
    
    if( sim_snapshot_buffer_.empty() ){
        return SimulationSnapshot();
    }

    if( shouldReturnEarliestSnapshot( render_time ) ){
        return sim_snapshot_buffer_.front();
    }

    if( shouldReturnLatestSnapshot( render_time ) ){
        return sim_snapshot_buffer_.back();
    }

    return computeInterpolatedSnapshot( render_time );
}

void StateInterpolator::setStaticSimulationData(const StaticSimulationData& data) {
    static_simulation_data = data;
}

const StaticSimulationData& StateInterpolator::getStaticSimulationData() const {
    return static_simulation_data;
}

void StateInterpolator::setSimulationSpeedMultiplier( double speed ){
    current_speed_multiplier_.store( speed );
}

// Returns the timestamp of the most recent snapshot in the buffer.
double StateInterpolator::getSimulationTime() const {
    lock_guard<mutex> lock( mutex_ );
    if( sim_snapshot_buffer_.empty() ){
        return 0.0;
    }
    return sim_snapshot_buffer_.back().simulation_time_;
}


double StateInterpolator::getSpeedMultiplier() const {
    return current_speed_multiplier_.load();
}

bool StateInterpolator::isSnapshotOutdated( const SimulationSnapshot& snapshot ) const {
    if( sim_snapshot_buffer_.empty() ){
        return false;
    }
    return snapshot.simulation_time_ < sim_snapshot_buffer_.back().simulation_time_;
}


void StateInterpolator::storeSnapshot( const SimulationSnapshot& snapshot ){
    sim_snapshot_buffer_.push_back( snapshot );
}

// Removes old snapshots from the front of the buffer to prevent unlimited memory growth.
// Keeps only the most recent snapshots needed for interpolation.
void StateInterpolator::enforceBufferSizeLimit(){
    while( sim_snapshot_buffer_.size() > MAX_BUFFER_SIZE ){
        sim_snapshot_buffer_.pop_front();
    }
}

// Checks if the requested time is before or at the first snapshot.
// In this case, return the earliest snapshot without interpolation.
bool StateInterpolator::shouldReturnEarliestSnapshot( double render_time ) const {
    return sim_snapshot_buffer_.size() == 1 || render_time <= sim_snapshot_buffer_.front().simulation_time_;
}


bool StateInterpolator::shouldReturnLatestSnapshot( double render_time ) const {
    return render_time >= sim_snapshot_buffer_.back().simulation_time_;
}

// Creates a blended snapshot for the requested time by finding the two snapshots
// that surround it and mixing them proportionally. Core interpolation logic.
SimulationSnapshot StateInterpolator::computeInterpolatedSnapshot( double render_time ) const {
    auto target_it = findFirstSnapshotAfter( render_time );
    
    if( target_it == sim_snapshot_buffer_.begin() ){
        return sim_snapshot_buffer_.front();
    }

    const SimulationSnapshot& snapshot_after = *target_it;
    const SimulationSnapshot& snapshot_before = *prev( target_it );

    double alpha = calculateInterpolationAlpha( snapshot_before, snapshot_after, render_time );

    return blendSnapshots( snapshot_before, snapshot_after, alpha, render_time );
}


deque<SimulationSnapshot>::const_iterator StateInterpolator::findFirstSnapshotAfter( double time ) const {
    return lower_bound( sim_snapshot_buffer_.begin(), sim_snapshot_buffer_.end(), time,
        []( const SimulationSnapshot& s, double t ){
            return s.simulation_time_ < t;
        });
}

// Calculates "alpha" - a value between 0.0 and 1.0 that represents how far the
// render time is between two snapshots. Alpha of 0.0 means use the first snapshot,
// 1.0 means use the second, 0.5 means blend them 50/50. This is the blend factor.
double StateInterpolator::calculateInterpolationAlpha( const SimulationSnapshot& before, const SimulationSnapshot& after, double render_time ) const {
    double duration = after.simulation_time_ - before.simulation_time_;
    
    if( duration <= 0.0 ) return 0.0;
    
    double alpha = ( render_time - before.simulation_time_ ) / duration;
    return clamp( alpha, 0.0, 1.0 );
}

// Mixes two snapshots together based on the blend factor (alpha).
// Creates a new snapshot with blended position and angle. The lawn state and points
// are copied from the end snapshot (no blending needed for discrete data).
SimulationSnapshot StateInterpolator::blendSnapshots( const SimulationSnapshot& start, const SimulationSnapshot& end, double alpha, double render_time ) const {
    SimulationSnapshot result = end; 
    
    result.x_ = interpolate( start.x_, end.x_, alpha );
    result.y_ = interpolate( start.y_, end.y_, alpha );
    result.angle_ = interpolateAngle( start.angle_, end.angle_, alpha );
    result.simulation_time_ = render_time;
    
    return result;
}

// Basic linear interpolation formula: start + (end - start) * blend_factor.
// When blend_factor is 0, returns start. When 1, returns end. In between, blends them.
double StateInterpolator::interpolate( double a, double b, double alpha ){ 
    return a + ( b - a ) * alpha;
}

// Special interpolation for angles that handles wrapping around 360 degrees.
// Without this, rotating from 350° to 10° would go the long way (340° backwards)
// instead of the short way (20° forward). 
double StateInterpolator::interpolateAngle( double start_angle, double end_angle, double alpha ){
    double diff = end_angle - start_angle;
    
    while( diff < -180.0 ) diff += 360.0;
    while( diff > 180.0 ) diff -= 360.0;
    
    return start_angle + diff * alpha;
}
