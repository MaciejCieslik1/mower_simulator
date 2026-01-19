/* 
    Author: Hanna Biegacz
    
    Tests Point struct methods.
*/
#include <gtest/gtest.h>
#include <cmath>
#include "../include/Point.h"
#include "../include/Constants.h"
using namespace std;

TEST(constructor, constructorSetsCoordinates) {
    double x = 1500.5;
    double y = 2300.7;
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(constructor, constructorOriginPoint) {
    Point point = Point(0.0, 0.0);

    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
}

TEST(constructor, constructorZeroXPositiveY) {
    Point point = Point(0.0, 500.0);

    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(500.0, point.getY());
}

TEST(constructor, constructorPositiveXZeroY) {
    Point point = Point(500.0, 0.0);

    EXPECT_DOUBLE_EQ(500.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
}

TEST(constructor, constructorMaximalValues) {
    double x = static_cast<double>(Constants::MAX_LAWN_WIDTH);
    double y = static_cast<double>(Constants::MAX_LAWN_LENGTH);
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(constructor, constructorVerySmallPositiveValues) {
    double x = 0.0001;
    double y = 0.0001;
    
    Point point = Point(x, y);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
}

TEST(constructor, constructorThrowsErrOnNegativeX) {
    EXPECT_THROW({ Point point = Point(-0.1, 100.0); }, std::invalid_argument);
}

TEST(constructor, constructorThrowsErrOnVerySmallNegativeX) {
    EXPECT_THROW({  Point point = Point(-0.0001, 100.0); }, std::invalid_argument);
}

TEST(constructor, constructorThrowsErrOnNegativeY) {
    EXPECT_THROW({ Point point = Point(100.0, -0.1); }, std::invalid_argument);
}

TEST(constructor, constructorThrowsErrOnVerySmallNegativeY) {
    EXPECT_THROW({ Point point = Point(100.0, -0.0001); }, std::invalid_argument);
}

TEST(constructor, constructorThrowsErrOnBothNegative) {
    EXPECT_THROW({ Point point = Point(-100.0, -200.0); }, std::invalid_argument);
}





TEST(idCounter, autoIncrementId) {
    Point first = Point(0.0, 0.0);
    Point second = Point(0.0, 0.0);
    Point third = Point(100.0, 200.0);

    EXPECT_LT(first.getId(), second.getId());
    EXPECT_LT(second.getId(), third.getId());
    EXPECT_EQ(second.getId(), first.getId() + 1);
    EXPECT_EQ(third.getId(), second.getId() + 1);
}

TEST(idCounter, eachPointHasUniqueId) {
    Point p1 = Point(0.0, 0.0);
    Point p2 = Point(0.0, 0.0);
    Point p3 = Point(100.0, 100.0);
    
    EXPECT_NE(p1.getId(), p2.getId());
    EXPECT_NE(p1.getId(), p3.getId());
    EXPECT_NE(p2.getId(), p3.getId());
}





TEST(calcDistanceTo, distanceFromOriginToOrigin) {
    Point p1 = Point(0.0, 0.0);
    Point p2 = Point(0.0, 0.0);

    double distance = p1.calcDistanceTo(p2);

    EXPECT_DOUBLE_EQ(0.0, distance);
}

TEST(calcDistanceTo, distanceBetweenOriginAndPoint) {
    Point origin = Point(0.0, 0.0);
    Point target = Point(300.0, 400.0);
    double expected_distance = 500.0; 

    double distance = origin.calcDistanceTo(target);

    EXPECT_DOUBLE_EQ(expected_distance, distance);
}

TEST(calcDistanceTo, distanceBetweenTwoPoints) {
    Point p1 = Point(100.0, 200.0);
    Point p2 = Point(400.0, 600.0);
    double dx = 300.0;
    double dy = 400.0;
    double expected_distance = sqrt(dx * dx + dy * dy);

    double distance = p1.calcDistanceTo(p2);

    EXPECT_DOUBLE_EQ(expected_distance, distance);
}

TEST(calcDistanceTo, distanceIsSymmetric) {
    Point p1 = Point(1500.5, 2000.7);
    Point p2 = Point(3000.3, 4500.9);

    double distance1 = p1.calcDistanceTo(p2);
    double distance2 = p2.calcDistanceTo(p1);

    EXPECT_DOUBLE_EQ(distance1, distance2);
}

TEST(calcDistanceTo, distanceOnSameXAxis) {
    Point p1 = Point(100.0, 500.0);
    Point p2 = Point(400.0, 500.0);
    double expected_distance = 300.0;
    
    double distance = p1.calcDistanceTo(p2);

    EXPECT_DOUBLE_EQ(expected_distance, distance);
}

TEST(calcDistanceTo, distanceOnSameYAxis) {
    Point p1 = Point(500.0, 100.0);
    Point p2 = Point(500.0, 700.0);
    double expected_distance = 600.0;
    
    double distance = p1.calcDistanceTo(p2);
    
    EXPECT_DOUBLE_EQ(expected_distance, distance);
}

TEST(calcDistanceTo, distanceFromOriginAlongXAxis) {
    Point origin = Point(0.0, 0.0);
    Point target = Point(100.0, 0.0);
    
    double distance = origin.calcDistanceTo(target);
    
    EXPECT_DOUBLE_EQ(100.0, distance);
}

TEST(calcDistanceTo, distanceFromOriginAlongYAxis) {
    Point origin = Point(0.0, 0.0);
    Point target = Point(0.0, 100.0);
    
    double distance = origin.calcDistanceTo(target);
    
    EXPECT_DOUBLE_EQ(100.0, distance);
}

TEST(calcDistanceTo, distanceWithVerySmallDifference) {
    Point p1 = Point(1000.0, 2000.0);
    Point p2 = Point(1000.01, 2000.01);
    double dx = 0.01;
    double dy = 0.01;
    double expected_distance = sqrt(dx * dx + dy * dy);
    
    double distance = p1.calcDistanceTo(p2);
    
    EXPECT_NEAR(expected_distance, distance, 1e-10);
}

TEST(calcDistanceTo, distanceWithMaximalCoordinates) {
    Point p1 = Point(0.0, 0.0);
    Point p2 = Point(   static_cast<double>(Constants::MAX_LAWN_WIDTH), 
                        static_cast<double>(Constants::MAX_LAWN_LENGTH));
    double max_width = static_cast<double>(Constants::MAX_LAWN_WIDTH);
    double max_length = static_cast<double>(Constants::MAX_LAWN_LENGTH);

    double expected_distance = sqrt(max_width * max_width + max_length * max_length);
    
    double distance = p1.calcDistanceTo(p2);
    
    EXPECT_DOUBLE_EQ(expected_distance, distance);
}





TEST(EqualityOperator, pointsWithSameCoordinatesAreEqual) {
    Point p1 = Point(1500.5, 2000.7);
    Point p2 = Point(1500.5, 2000.7);
    
    EXPECT_TRUE(p1 == p2);
}

TEST(EqualityOperator, originPointsAreEqual) {
    Point p1 = Point(0.0, 0.0);
    Point p2 = Point(0.0, 0.0);
    
    EXPECT_TRUE(p1 == p2);
}

TEST(EqualityOperator, pointsWithDifferentCoordinatesAreNotEqual) {
    Point p1 = Point(1500.5, 2000.7);
    Point p2 = Point(1500.6, 2000.7);
    
    EXPECT_FALSE(p1 == p2);
}

TEST(EqualityOperator, pointsWithVeryCloseCoordinatesAreEqual) {
    Point p1 = Point(1000.0, 2000.0);
    Point p2 = Point(1000.0000000001, 2000.0000000001);
    
    EXPECT_TRUE(p1 == p2);
}
TEST(EqualityOperator, pointsWithDifferenceJustAboveEpsilonNotEqual) {
    Point p1 = Point(1000.0, 2000.0);
    Point p2 = Point(1000.00000001, 2000.0);  // above EPSILON
    
    EXPECT_FALSE(p1 == p2);
}
TEST(EqualityOperator, equalityIgnoresId) {
    Point p1 = Point(100.0, 200.0);
    Point p2 = Point(100.0, 200.0);
    
    EXPECT_NE(p1.getId(), p2.getId());
    EXPECT_TRUE(p1 == p2);
}

TEST(EqualityOperator, pointsWithDifferentXCoordinatesNotEqual) {
    Point p1 = Point(100.0, 200.0);
    Point p2 = Point(101.0, 200.0);
    
    EXPECT_FALSE(p1 == p2);
}

TEST(EqualityOperator, pointsWithDifferentYCoordinatesNotEqual) {
    Point p1 = Point(100.0, 200.0);
    Point p2 = Point(100.0, 201.0);
    
    EXPECT_FALSE(p1 == p2);
}