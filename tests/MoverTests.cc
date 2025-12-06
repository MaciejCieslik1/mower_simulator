/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include "../include/Config.h"
#include "../include/Mover.h"
#include "../include/Exceptions.h"

using namespace std;


TEST(ConstructorAndGetters, constructorAndGetters) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);

    unsigned int result_width = mover.getWidth();
    unsigned int result_length = mover.getLength();
    unsigned int result_blade_diameter = mover.getBladeDiameter();
    unsigned int result_speed = mover.getSpeed();
    unsigned short result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(width, result_width);
    EXPECT_EQ(length, result_length);
    EXPECT_EQ(blade_diameter, result_blade_diameter);
    EXPECT_EQ(speed, result_speed);
    EXPECT_EQ(0, result_angle);
    EXPECT_NEAR(0.0, result_x, 1e-9);
    EXPECT_NEAR(0.0, result_y, 1e-9);
}


TEST(Setters, setters) {
    unsigned short angle = 120;
    double x = 4.0;
    double y = 32;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMoverConstants(width, length, 0.0, 0.0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.setAngle(angle);
    mover.setX(x);
    mover.setY(y);

    unsigned int result_angle = mover.getAngle();
    unsigned int result_x = mover.getX();
    unsigned int result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-9);
    EXPECT_NEAR(y, result_y, 1e-9);
}


TEST(Move, moveInsideLawnDefaultSpawnPoint) {
    unsigned short angle = 45;
    double distance = 100;
    double x = 70.711;
    double y = 70.711;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnDoubleDistanceSpawnPoint) {
    unsigned short angle = 45;
    double distance = 50.345;
    double x = 35.599;
    double y = 35.599;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnHardAngleDefaultSpawnPoint) {
    unsigned short angle = 37;
    double distance = 100;
    double x = 60.1815;
    double y = 79.8635;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnHardAngleDoubleDistanceDefaultSpawnPoint) {
    unsigned short angle = 37;
    double distance = 50.345;
    double x = 30.2983;
    double y = 40.2073;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0.0, 0.0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnHardAngleDoubleDistanceCustomSpawnPoint) {
    unsigned short angle = 37;
    double distance = 50.345;
    double x = 60.596;
    double y = 80.414;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 30.298, 40.207, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnBorderXDefaultSpawnPoint) {
    unsigned short angle = 90;
    double distance = 100;
    double x = 100;
    double y = 0;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveInsideLawnBorderYDefaultSpawnPoint) {
    unsigned short angle = 0;
    double distance = 100;
    double x = 0;
    double y = 100;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);
    mover.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mover.getAngle();
    double result_x = mover.getX();
    double result_y = mover.getY();

    EXPECT_EQ(angle, result_angle);
    EXPECT_NEAR(x, result_x, 1e-3);
    EXPECT_NEAR(y, result_y, 1e-3);
}


TEST(Move, moveOutsideLawnBorderYDefaultSpawnPointMinimalDeltaX) {
    unsigned short angle = 0;
    double distance = 105.001;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Move, moveOutsideLawnBorderYDefaultSpawnPointMinimalDeltaY) {
    unsigned short angle = 0;
    double distance = 120.0001;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Move, moveOutsideLawnBorderDefaultSpawnPoint) {
    unsigned short angle = 45;
    double distance = 200;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Move, moveOutsideLawnBorderDefaultSpawnPointNegativeX) {
    unsigned short angle = 270;
    double distance = 5.001;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Move, moveOutsideLawnBorderDefaultSpawnPointNegativeY) {
    unsigned short angle = 180;
    double distance = 5.001;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Move, moveOutsideLawnBorderDefaultSpawnPointNegativeXY) {
    unsigned short angle = 225;
    double distance = 10;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, angle);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
}


TEST(Rotate, rotatePositiveAngleDefaultSpawnAngle) {
    short angle_to_rotate = 90;
    unsigned short angle = 90;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateMaxPositiveAngleDefaultSpawnAngle) {
    short angle_to_rotate = 360;
    unsigned short angle = 0;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateNegativeAngleDefaultSpawnAngle) {
    short angle_to_rotate = -90;
    unsigned short angle = 270;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateMinNegativeAngleDefaultSpawnAngle) {
    short angle_to_rotate = -360;
    unsigned short angle = 0;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 0);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotatePositiveAngleCustomSpawnAngle) {
    short angle_to_rotate = 90;
    unsigned short angle = 165;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 75);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateNegativeAngleCustomSpawnAngle) {
    short angle_to_rotate = -90;
    unsigned short angle = 345;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 75);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateMaxPositiveAngleCustomSpawnAngle) {
    short angle_to_rotate = 360;
    unsigned short angle = 30;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 30);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateMinNegativeAngleCustomSpawnAngle) {
    short angle_to_rotate = -360;
    unsigned short angle = 30;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 30);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateCustomNegativeAngleCustomSpawnAngle) {
    short angle_to_rotate = -225;
    unsigned short angle = 165;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 30);
    Mover mover = Mover(width, length, blade_diameter, speed);

    mover.rotate(angle_to_rotate);
    unsigned short result_angle = mover.getAngle();

    EXPECT_EQ(angle, result_angle);
}


TEST(Rotate, rotateTooBigPositiveAngle) {
    short angle_to_rotate = 361;
    unsigned short angle = 165;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 30);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.rotate(angle_to_rotate);}, RotationAngleOutOfRangeError);
}


TEST(Rotate, rotateTooSmallNegativeAngle) {
    short angle_to_rotate = -361;
    unsigned short angle = 165;
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMoverConstants(width, length, 0, 0, 30);
    Mover mover = Mover(width, length, blade_diameter, speed);

    EXPECT_THROW({mover.rotate(angle_to_rotate);}, RotationAngleOutOfRangeError);
}
