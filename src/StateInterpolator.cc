/*
    Author: Hanna Biegacz
    Implementation of StateInterpolator.
*/

#include <algorithm> 
#include <cmath>     
#include <iterator>  
#include "StateInterpolator.h"

using namespace std;


void StateInterpolator::addSimulationSnapshot( const SimulationSnapshot& sim_snapshot ){
    lock_guard<mutex> lock( mutex_ );
    
    if( isSnapshotOutdatedOrDuplicate( sim_snapshot ) ){
        return;
    }
    
    storeSnapshot( sim_snapshot );
    enforceBufferSizeLimit();
}


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


bool StateInterpolator::isSnapshotOutdatedOrDuplicate( const SimulationSnapshot& snapshot ) const {
    if( sim_snapshot_buffer_.empty() ){
        return false;
    }
    return snapshot.simulation_time_ <= sim_snapshot_buffer_.back().simulation_time_;
}


void StateInterpolator::storeSnapshot( const SimulationSnapshot& snapshot ){
    sim_snapshot_buffer_.push_back( snapshot );
}


void StateInterpolator::enforceBufferSizeLimit(){
    while( sim_snapshot_buffer_.size() > MAX_BUFFER_SIZE ){
        sim_snapshot_buffer_.pop_front();
    }
}


bool StateInterpolator::shouldReturnEarliestSnapshot( double render_time ) const {
    return sim_snapshot_buffer_.size() == 1 || render_time <= sim_snapshot_buffer_.front().simulation_time_;
}


bool StateInterpolator::shouldReturnLatestSnapshot( double render_time ) const {
    return render_time >= sim_snapshot_buffer_.back().simulation_time_;
}


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


double StateInterpolator::calculateInterpolationAlpha( const SimulationSnapshot& before, const SimulationSnapshot& after, double render_time ) const {
    double duration = after.simulation_time_ - before.simulation_time_;
    
    if( duration <= 0.0 ) return 0.0;
    
    double alpha = ( render_time - before.simulation_time_ ) / duration;
    return clamp( alpha, 0.0, 1.0 );
}


SimulationSnapshot StateInterpolator::blendSnapshots( const SimulationSnapshot& start, const SimulationSnapshot& end, double alpha, double render_time ) const {
    SimulationSnapshot result = end; 
    
    result.x_ = interpolate( start.x_, end.x_, alpha );
    result.y_ = interpolate( start.y_, end.y_, alpha );
    result.angle_ = interpolateAngle( start.angle_, end.angle_, alpha );
    result.simulation_time_ = render_time;
    
    return result;
}


double StateInterpolator::interpolate( double a, double b, double alpha ){ 
    return a + ( b - a ) * alpha;
}


double StateInterpolator::interpolateAngle( double start_angle, double end_angle, double alpha ){
    double diff = end_angle - start_angle;
    
    while( diff < -180.0 ) diff += 360.0;
    while( diff > 180.0 ) diff -= 360.0;
    
    return start_angle + diff * alpha;
}
