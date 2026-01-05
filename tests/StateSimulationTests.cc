#include <gtest/gtest.h>
#include <cmath>
#include "../include/Constants.h"
#include "../include/Config.h"
#include "../include/Mover.h"
#include "../include/Lawn.h"
#include "../include/Log.h"
#include "../include/Logger.h"
#include "../include/StateSimulation.h"
#include "../include/Exceptions.h"

using namespace std;


TEST(ConstructorAndGetters, constructorAndGetters) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);

    bool result_lawn = lawn == stateSimulation.getLawn();
    bool result_mover = mover == stateSimulation.getMover();
    bool result_logger_size = logger.getLogs().size() == stateSimulation.getLogger().getLogs().size();
    uint64_t result_time = 0;

    EXPECT_TRUE(result_lawn);
    EXPECT_TRUE(result_mover);
    EXPECT_TRUE(result_logger_size);
    EXPECT_EQ(result_time, stateSimulation.getTime());
}


TEST(OperatorEquals, equals) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn, mover, logger);

    bool result_simulation = stateSimulation == stateSimulation2;

    EXPECT_TRUE(result_simulation);
}


TEST(OperatorEquals, notEqualsLawn) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width + 1, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn2, mover, logger);

    bool result_simulation = stateSimulation == stateSimulation2;

    EXPECT_FALSE(result_simulation);
}


TEST(OperatorNotEquals, notEqualsMover) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Mover mover2 = Mover(width + 1, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn, mover2, logger);

    bool result_simulation = stateSimulation == stateSimulation2;

    EXPECT_FALSE(result_simulation);
}


TEST(OperatorNotEquals, notEqualsLogger) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    Logger logger2 = Logger();
    logger2.push(Log(20, "Hello"));
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn, mover, logger2);

    bool result_simulation = stateSimulation == stateSimulation2;

    EXPECT_FALSE(result_simulation);
}


TEST(OperatorNotEquals, notEquals) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width + 1, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn2, mover, logger);

    bool result_simulation = stateSimulation != stateSimulation2;

    EXPECT_TRUE(result_simulation);
}


TEST(OperatorNotEquals, notEqualsEquals) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    StateSimulation stateSimulation2 = StateSimulation(lawn, mover, logger);

    bool result_simulation = stateSimulation != stateSimulation2;

    EXPECT_FALSE(result_simulation);
}


TEST(SimulateMovement, moveInsideLawn) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 500;
    double result_x = 0;
    double result_y = 500;
    unsigned short result_angle = 0;
    double time_ms = double(distance) * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 0;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateMovement, moveInsideLawnCorner) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 1000;
    double result_x = 0;
    double result_y = 1000;
    unsigned short result_angle = 0;
    double time_ms = double(distance) * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 0;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateMovement, moveOutsideLawnLine) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 1001;
    double result_x = 0;
    double result_y = 1000;
    unsigned short result_angle = 0;
    double time_ms = 1000.0 * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / Constants::TICK_DURATION) * Constants::TICK_DURATION);
    int result_logger_size = 1;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}

TEST(SimulateMovement, moveOutsideLawnCustomAngleCorner) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 500, 500, 45);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 1000;
    double result_x = 1000;
    double result_y = 1000;
    unsigned short result_angle = 45;
    double time_ms = double(distance / 2 * sqrt(2)) * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 1;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_NEAR(result_time, stateSimulation.getTime(), Constants::TICK_DURATION);
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateMovement, moveOutsideLawnCustomAngle) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 500, 0, 45);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 1000;
    double result_x = 1000;
    double result_y = 500;
    unsigned short result_angle = 45;
    double time_ms = double(distance / 2 * sqrt(2)) * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 1;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateMovement, moveOutsideLawnCustomAngleOver180) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 500, 500, 225);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    double distance = 1000;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 225;
    double time_ms = double(distance / 2 * sqrt(2)) * 1000.0 / mover.getSpeed();
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 1;

    stateSimulation.simulateMovement(distance);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateRotation, rotate) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    unsigned short rotation = 90;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 90;
    double time_ms = double(rotation / Constants::ROTATION_SPEED) * 1000;
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 0;

    stateSimulation.simulateRotation(rotation);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateRotation, rotatePass360) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 270);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    unsigned short rotation = 180;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 90;
    double time_ms = double(rotation / Constants::ROTATION_SPEED) * 1000;
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 0;

    stateSimulation.simulateRotation(rotation);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateRotation, rotatePass360NegativeAngle) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 90);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    short rotation = -180;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 270;
    double time_ms = double(abs(rotation) / Constants::ROTATION_SPEED) * 1000;
    uint64_t result_time = uint64_t(ceil(time_ms / 10.0) * 10.0);
    int result_logger_size = 0;

    stateSimulation.simulateRotation(rotation);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateRotation, invalidAngleTooBig) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 90);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    unsigned short rotation = 361;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 90;
    uint64_t result_time = 0;
    int result_logger_size = 1;

    stateSimulation.simulateRotation(rotation);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}


TEST(SimulateRotation, invalidAngleTooSmall) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 90);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mover mover = Mover(width, length, blade_diameter, speed);
    Logger logger = Logger();
    StateSimulation stateSimulation = StateSimulation(lawn, mover, logger);
    unsigned short rotation = -361;
    double result_x = 0;
    double result_y = 0;
    unsigned short result_angle = 90;
    uint64_t result_time = 0;
    int result_logger_size = 1;

    stateSimulation.simulateRotation(rotation);

    EXPECT_NEAR(result_x, stateSimulation.getMover().getX(), Constants::DISTANCE_PRECISION);
    EXPECT_NEAR(result_y, stateSimulation.getMover().getY(), Constants::DISTANCE_PRECISION);
    EXPECT_EQ(result_angle, mover.getAngle());
    EXPECT_EQ(result_time, stateSimulation.getTime());
    EXPECT_EQ(result_logger_size, stateSimulation.getLogger().getLogs().size());
}
