#pragma once

#include <queue>
#include <memory>
#include "Commands.h"
#include "StateSimulation.h"

class MowerController {
public:
    MowerController() = default;

    void move(double cm);
    void move(const double* distance_ptr, double scale = 1.0);
    void rotate(short deg);
    void setMowing(bool enable);
    void addPoint(double x, double y);
    void deletePoint(unsigned int point_id);
    void moveToPoint(unsigned int point_id);
    void getDistanceToPoint(unsigned int point_id, double& out_distance);
    void rotateTowardsPoint(unsigned int point_id);


    void update(StateSimulation& sim, double dt);

    bool hasCommands() const;

private:
    std::queue<std::unique_ptr<ICommand>> command_queue_;
};
