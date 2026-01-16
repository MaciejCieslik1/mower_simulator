#pragma once
#include <vector>
#include "Point.h"

struct SimulationSnapshot { 
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
