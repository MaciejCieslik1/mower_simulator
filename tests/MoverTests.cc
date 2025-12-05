/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include "../include/Mover.h"
#include "../include/Config.h"

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
