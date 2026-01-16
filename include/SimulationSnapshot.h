#pragma once
#include <vector>
#include "Point.h"

struct SimulationSnapshot { 
    double x_ = 0;
    double y_ = 0;
    double angle_ = 0;
    double simulation_time_ = 0;

    std::vector<std::vector<bool>> fields_; //TODO: kopiowanie tego jest drogie!
    std::vector<Point> points_;
};
