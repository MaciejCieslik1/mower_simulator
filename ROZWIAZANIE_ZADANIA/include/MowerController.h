/*
    Author: Hanna Biegacz
    This class is responsible for handling user input and executing commands.
    Manages a command queue for the lawn mower.
    Provides simple methods to control the mower (move, rotate, mowing on/off)
    and executes commands sequentially during simulation updates.
*/

#pragma once

#include <queue>
#include <memory>
#include "StateSimulation.h"
#include "commands/AddPointCommand.h"
#include "commands/DeletePointCommand.h"
#include "commands/MoveCommand.h"
#include "commands/MoveToPointCommand.h"
#include "commands/GetDistanceToPointCommand.h"
#include "commands/RotateCommand.h"
#include "commands/RotateTowardsPointCommand.h"
#include "commands/MowingOptionCommand.h"
#include "commands/GetCurrentAngleCommand.h"
#include "commands/GetCurrentPositionCommand.h"

class MowerController {
public:
    MowerController() = default;
    ~MowerController() = default;
    MowerController(const MowerController&) = delete;
    MowerController& operator=(const MowerController&) = delete;

    void move(double cm);
    void move(const double* distance_ptr, double scale = 1.0);
    void rotate(short deg);
    void setMowing(bool enable);
    void addPoint(double x, double y);
    void deletePoint(unsigned int point_id);
    void moveToPoint(unsigned int point_id);
    void getDistanceToPoint(unsigned int point_id, double& out_distance);
    void rotateTowardsPoint(unsigned int point_id);
    void getCurrentAngle(unsigned short& out_angle);
    void getCurrentPosition(double& out_x, double& out_y);



    void update(StateSimulation& sim, double dt);

private:
    std::queue<std::unique_ptr<ICommand>> command_queue_;
};
