/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include "../include/Config.h"
#include "../include/Mower.h"
#include "../include/Exceptions.h"

using namespace std;


TEST(ConstructorAndGetters, constructorAndGetters) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);

    unsigned int result_width = mower.getWidth();
    unsigned int result_length = mower.getLength();
    unsigned int result_blade_diameter = mower.getBladeDiameter();
    unsigned int result_speed = mower.getSpeed();
    unsigned short result_angle = mower.getAngle();
    bool result_is_mowing = mower.getIsMowing();
    double result_x = mower.getX();
    double result_y = mower.getY();

    EXPECT_EQ(width, result_width);
    EXPECT_EQ(length, result_length);
    EXPECT_EQ(blade_diameter, result_blade_diameter);
    EXPECT_EQ(speed, result_speed);
    EXPECT_EQ(0, result_angle);
    EXPECT_TRUE(result_is_mowing);
    EXPECT_NEAR(0.0, result_x, 1e-9);
    EXPECT_NEAR(0.0, result_y, 1e-9);
}


TEST(OperatorEquals, equals) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_TRUE(result);
}


TEST(OperatorEquals, notEqualsWidth) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width + 1, length, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsLength) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length + 1, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsBladeDiameter) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length, blade_diameter + 1, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsSpeed) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length, blade_diameter, speed + 1);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsX) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.setX(mower.getX() + 1);
    Mower mower2 = Mower(width, length, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsY) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.setY(mower.getY() + 1);
    Mower mower2 = Mower(width, length, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsAngle) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.setAngle(mower.getAngle() + 1);
    Mower mower2 = Mower(width, length, blade_diameter, speed);

    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsIsMowing) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length, blade_diameter, speed);
    mower.turnOffMowing();
    
    bool result = mower == mower2;
    EXPECT_FALSE(result);
}


TEST(OperatorNotEquals, notEquals) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width + 1, length, blade_diameter, speed);

    bool result = mower != mower2;
    EXPECT_TRUE(result);
}


TEST(OperatorNotEquals, equals) {
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    Mower mower2 = Mower(width, length, blade_diameter, speed);

    bool result = mower != mower2;
    EXPECT_FALSE(result);
}


TEST(Setters, setters) {
    unsigned short angle = 120;
    double x = 4.0;
    double y = 32;
    unsigned int width = 120;
    unsigned int length = 100;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    Config::initializeMowerConstants(width, length, 0.0, 0.0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.setAngle(angle);
    mower.setX(x);
    mower.setY(y);

    unsigned int result_angle = mower.getAngle();
    unsigned int result_x = mower.getX();
    unsigned int result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0.0, 0.0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0.0, 0.0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0.0, 0.0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0.0, 0.0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 30.298, 40.207, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.move(distance, lawn_width, lawn_length);

    unsigned int result_angle = mower.getAngle();
    double result_x = mower.getX();
    double result_y = mower.getY();

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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, angle);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.move(distance, lawn_width, lawn_length);}, MoveOutsideLawnError);
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
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 75);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 75);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 30);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 30);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 30);
    Mower mower = Mower(width, length, blade_diameter, speed);

    mower.rotate(angle_to_rotate);
    unsigned short result_angle = mower.getAngle();

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
    Config::initializeMowerConstants(width, length, 0, 0, 30);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.rotate(angle_to_rotate);}, RotationAngleOutOfRangeError);
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
    Config::initializeMowerConstants(width, length, 0, 0, 30);
    Mower mower = Mower(width, length, blade_diameter, speed);

    EXPECT_THROW({mower.rotate(angle_to_rotate);}, RotationAngleOutOfRangeError);
}


TEST(TurnOnMowing, turnOn) {
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.turnOffMowing();
    mower.turnOnMowing();

    EXPECT_TRUE(mower.getIsMowing());
}


TEST(TurnOffMowing, turnOff) {
    unsigned int width = 10;
    unsigned int length = 10;
    unsigned int blade_diameter = 90;
    unsigned int speed = 105;
    unsigned int lawn_width = 120;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(width, length, 0, 0, 0);
    Mower mower = Mower(width, length, blade_diameter, speed);
    mower.turnOffMowing();

    EXPECT_FALSE(mower.getIsMowing());
}
