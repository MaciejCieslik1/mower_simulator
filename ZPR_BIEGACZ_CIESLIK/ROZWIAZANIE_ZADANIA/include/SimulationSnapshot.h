/*
    Author: Hanna Biegacz
    
    SimulationSnapshot stores a single frame of simulation state data.
    Used by StateInterpolator to perform smooth rendering without 
    repeatedly locking and accessing the main StateSimulation object.
    Contains mower position, lawn state, and points at a specific time.
*/

#pragma once
#include <vector>
#include "Point.h"

struct SimulationSnapshot { 
    double x_ = 0;
    double y_ = 0;
    double angle_ = 0;
    double simulation_time_ = 0;

    std::vector<std::vector<bool>> fields_;
    std::vector<Point> points_;
};
