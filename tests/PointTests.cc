/* 
    Author: Hanna Biegacz
    
    Tests Point struct methods.
*/
#include <gtest/gtest.h>
#include <cmath>
#include "../include/Point.h"
#include "../include/Constants.h"
using namespace std;

TEST(Constructor, constructorSetsCoordinates) {
    double x = 1500.5;
    double y = 2300.7;
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(Constructor, constructorOriginPoint) {
    Point point = Point(0.0, 0.0);

    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
}

TEST(Constructor, constructorZeroXPositiveY) {
    Point point = Point(0.0, 500.0);

    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(500.0, point.getY());
}

TEST(Constructor, constructorPositiveXZeroY) {
    Point point = Point(500.0, 0.0);

    EXPECT_DOUBLE_EQ(500.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
}

TEST(Constructor, constructorMaximalValues) {
    double x = static_cast<double>(Constants::MAX_LAWN_WIDTH);
    double y = static_cast<double>(Constants::MAX_LAWN_LENGTH);
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(Constructor, constructorVerySmallPositiveValues) {
    double x = 0.0001;
    double y = 0.0001;
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(Constructor, constructorThrowsErrOnNegativeX) {
    EXPECT_THROW({ Point point = Point(-0.1, 100.0); }, std::invalid_argument);
}

TEST(Constructor, constructorThrowsErrOnVerySmallNegativeX) {
    EXPECT_THROW({  Point point = Point(-0.0001, 100.0); }, std::invalid_argument);
}

TEST(Constructor, constructorThrowsErrOnNegativeY) {
    EXPECT_THROW({ Point point = Point(100.0, -0.1); }, std::invalid_argument);
}

TEST(Constructor, constructorThrowsErrOnVerySmallNegativeY) {
    EXPECT_THROW({ Point point = Point(100.0, -0.0001); }, std::invalid_argument);
}

TEST(Constructor, constructorThrowsErrOnBothNegative) {
    EXPECT_THROW({ Point point = Point(-100.0, -200.0); }, std::invalid_argument);
}

