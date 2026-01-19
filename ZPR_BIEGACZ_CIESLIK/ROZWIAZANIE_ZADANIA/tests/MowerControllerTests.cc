#include <gtest/gtest.h>
#include "StateSimulation.h"
#include "MowerController.h"
#include "Config.h"

TEST(MowerControllerUpdate, updateExecutesNoCommandsWhenQueueIsEmpty) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double delta_time = 0.016;

    controller.update(stateSimulation, delta_time);

    EXPECT_EQ(0, stateSimulation.getTime());
}

TEST(MowerControllerMove, moveAddsCommandToQueue) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double move_distance_cm = 100.0;
    double delta_time = 10.0;

    controller.move(move_distance_cm);
    controller.update(stateSimulation, delta_time);

    EXPECT_GT(stateSimulation.getTime(), 0);
}

TEST(MowerControllerMove, moveWithPointerAddsCommandToQueue) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double move_distance_cm = 100.0;
    double scale_factor = 1.0;
    double delta_time = 10.0;

    controller.move(&move_distance_cm, scale_factor);
    controller.update(stateSimulation, delta_time);

    EXPECT_GT(stateSimulation.getTime(), 0);
}

TEST(MowerControllerRotate, rotateAddsCommandToQueue) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    short rotation_angle_degrees = 90;
    double delta_time = 10.0;
    unsigned short initial_mower_angle = stateSimulation.getMower().getAngle();

    controller.rotate(rotation_angle_degrees);
    controller.update(stateSimulation, delta_time);

    EXPECT_NE(initial_mower_angle, stateSimulation.getMower().getAngle());
}

TEST(MowerControllerSetMowing, setMowingEnableTurnsOnMowing) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    mower.turnOffMowing();
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    bool enable_mowing = true;
    double delta_time = 0.016;

    controller.setMowing(enable_mowing);
    controller.update(stateSimulation, delta_time);

    EXPECT_TRUE(stateSimulation.getMower().getIsMowing());
}

TEST(MowerControllerSetMowing, setMowingDisableTurnsOffMowing) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    bool disable_mowing = false;
    double delta_time = 0.016;

    controller.setMowing(disable_mowing);
    controller.update(stateSimulation, delta_time);

    EXPECT_FALSE(stateSimulation.getMower().getIsMowing());
}

TEST(MowerControllerAddPoint, addPointAddsPointToSimulation) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double point_x_coordinate = 300.0;
    double point_y_coordinate = 400.0;
    double delta_time = 0.016;
    size_t initial_points_count = stateSimulation.getPoints().size();

    controller.addPoint(point_x_coordinate, point_y_coordinate);
    controller.update(stateSimulation, delta_time);

    EXPECT_EQ(initial_points_count + 1, stateSimulation.getPoints().size());
}

TEST(MowerControllerDeletePoint, deletePointRemovesPointFromSimulation) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double point_x_coordinate = 300.0;
    double point_y_coordinate = 400.0;
    double delta_time = 0.016;
    unsigned int point_id_to_delete = 0;

    controller.addPoint(point_x_coordinate, point_y_coordinate);
    controller.update(stateSimulation, delta_time);
    size_t points_count_after_adding = stateSimulation.getPoints().size();
    controller.deletePoint(point_id_to_delete);
    controller.update(stateSimulation, delta_time);

    EXPECT_EQ(points_count_after_adding - 1, stateSimulation.getPoints().size());
}

TEST(MowerControllerMoveToPoint, moveToPointMovesNearPointLocation) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 100.0, 100.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double target_point_x = 500.0;
    double target_point_y = 500.0;
    double delta_time = 100.0;
    unsigned int target_point_id = 0;

    controller.addPoint(target_point_x, target_point_y);
    controller.update(stateSimulation, delta_time);
    double initial_x = stateSimulation.getMower().getX();
    double initial_y = stateSimulation.getMower().getY();
    controller.moveToPoint(target_point_id);
    
    int max_steps = 100;
    while (stateSimulation.getMower().getX() == initial_x && 
           stateSimulation.getMower().getY() == initial_y && 
           max_steps > 0) {
        controller.update(stateSimulation, delta_time);
        max_steps--;
    }

    EXPECT_NE(initial_x, stateSimulation.getMower().getX());
    EXPECT_NE(initial_y, stateSimulation.getMower().getY());
}

TEST(MowerControllerGetDistanceToPoint, getDistanceToPointCalculatesCorrectDistance) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double point_x_coordinate = 300.0;
    double point_y_coordinate = 400.0;
    double output_distance = 0.0;
    double delta_time = 0.016;
    unsigned int point_id = 0;

    controller.addPoint(point_x_coordinate, point_y_coordinate);
    controller.update(stateSimulation, delta_time);
    controller.getDistanceToPoint(point_id, output_distance);
    controller.update(stateSimulation, delta_time);

    EXPECT_EQ(500.0, output_distance);
}

TEST(MowerControllerRotateTowardsPoint, rotateTowardsPointChangesAngle) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 0.0, 0.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double point_x_coordinate = 500.0;
    double point_y_coordinate = 500.0;
    double delta_time = 10.0;
    unsigned int point_id = 0;
    unsigned short initial_angle = stateSimulation.getMower().getAngle();

    controller.addPoint(point_x_coordinate, point_y_coordinate);
    controller.update(stateSimulation, delta_time);
    controller.rotateTowardsPoint(point_id);
    controller.update(stateSimulation, delta_time);

    EXPECT_NE(initial_angle, stateSimulation.getMower().getAngle());
}

TEST(MowerControllerUpdate, updateExecutesMultipleCommandsInOrder) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 500.0, 500.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    short first_rotation_degrees = 45;
    double move_distance_cm = 50.0;
    short second_rotation_degrees = -90;
    double delta_time = 100.0;

    controller.rotate(first_rotation_degrees);
    controller.move(move_distance_cm);
    controller.rotate(second_rotation_degrees);
    controller.update(stateSimulation, delta_time);

    EXPECT_GT(stateSimulation.getTime(), 0);
}

TEST(MowerControllerGetCurrentPosition, getCurrentPositionRetrievesMowerPosition) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    unsigned int mower_width = 120;
    unsigned int mower_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 450.0, 550.0, 0);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Mower mower = Mower(mower_width, mower_length, blade_diameter, speed);
    Logger logger = Logger();
    FileLogger fileLogger = FileLogger("test_path");
    StateSimulation stateSimulation = StateSimulation(lawn, mower, logger, fileLogger);
    MowerController controller = MowerController();
    double out_x = 0.0, out_y = 0.0;
    double delta_time = 0.016;

    controller.getCurrentPosition(out_x, out_y);
    controller.update(stateSimulation, delta_time);

    EXPECT_DOUBLE_EQ(450.0, out_x);
    EXPECT_DOUBLE_EQ(550.0, out_y);
}
