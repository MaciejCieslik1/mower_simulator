/* 
    Author: Hanna Biegacz, updated: Maciej Cieślik
    
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
    unsigned int id = 0;
    
    Point point = Point(x, y, id);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
    EXPECT_EQ(id, point.getId());
}

TEST(constructor, constructorOriginPoint) {
    unsigned int id = 0;
    Point point = Point(0.0, 0.0, id);


    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
    EXPECT_EQ(id, point.getId());
}

TEST(constructor, constructorZeroXPositiveY) {
    unsigned int id = 1;
    Point point = Point(0.0, 500.0, id);

    EXPECT_DOUBLE_EQ(0.0, point.getX());
    EXPECT_DOUBLE_EQ(500.0, point.getY());
    EXPECT_EQ(id, point.getId());
}

TEST(constructor, constructorPositiveXZeroY) {
    Point point = Point(500.0, 0.0, 1);
    unsigned int id = 1;

    EXPECT_DOUBLE_EQ(500.0, point.getX());
    EXPECT_DOUBLE_EQ(0.0, point.getY());
    EXPECT_EQ(id, point.getId());
}

TEST(constructor, constructorMaximalValues) {
    double x = static_cast<double>(Constants::MAX_LAWN_WIDTH);
    double y = static_cast<double>(Constants::MAX_LAWN_LENGTH);
    unsigned int id = 1;
    
    Point point = Point(x, y, id);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
    EXPECT_EQ(id, point.getId());
}

TEST(constructor, constructorVerySmallPositiveValues) {
    double x = 0.0001;
    double y = 0.0001;
    unsigned int id = 1;
    
    Point point = Point(x, y, id);

    EXPECT_DOUBLE_EQ(x, point.getX());
    EXPECT_DOUBLE_EQ(y, point.getY());
    EXPECT_EQ(id, point.getId());
}


TEST(EqualityOperator, pointsWithSameCoordinatesAreEqual) {
    Point p1 = Point(1500.5, 2000.7, 0);
    Point p2 = Point(1500.5, 2000.7, 0);
    
    EXPECT_TRUE(p1 == p2);
}


TEST(EqualityOperator, originPointsAreEqual) {
    Point p1 = Point(0.0, 0.0, 0);
    Point p2 = Point(0.0, 0.0, 0);
    
    EXPECT_TRUE(p1 == p2);
}


TEST(EqualityOperator, originPointsDifferentIdAreNotEqual) {
    Point p1 = Point(0.0, 0.0, 0);
    Point p2 = Point(0.0, 0.0, 1);
    
    EXPECT_FALSE(p1 == p2);
}


TEST(EqualityOperator, pointsWithDifferentCoordinatesAreNotEqual) {
    Point p1 = Point(1500.5, 2000.7, 1);
    Point p2 = Point(1500.6, 2000.7, 1);
    
    EXPECT_FALSE(p1 == p2);
}


TEST(EqualityOperator, pointsWithVeryCloseCoordinatesAreEqual) {
    Point p1 = Point(1000.0, 2000.0, 0);
    Point p2 = Point(1000.0001, 2000.0001, 0);
    
    EXPECT_TRUE(p1 == p2);
}


TEST(EqualityOperator, pointsWithDifferenceJustAboveEpsilonNotEqual) {
    Point p1 = Point(1000.0, 2000.0, 0);
    Point p2 = Point(1000.002, 2000.0, 0);
    
    EXPECT_FALSE(p1 == p2);
}


TEST(EqualityOperator, pointsWithDifferentXCoordinatesNotEqual) {
    Point p1 = Point(100.0, 200.0, 0);
    Point p2 = Point(101.0, 200.0, 0);
    
    EXPECT_FALSE(p1 == p2);
}


TEST(EqualityOperator, pointsWithDifferentYCoordinatesNotEqual) {
    Point p1 = Point(100.0, 200.0, 0);
    Point p2 = Point(100.0, 201.0, 0);
    
    EXPECT_FALSE(p1 == p2);
}


TEST(NotEqualOperator, notEqual) {
    Point p1 = Point(100.0, 200.0, 0);
    Point p2 = Point(100.0, 201.0, 0);
    
    EXPECT_TRUE(p1 != p2);
}


TEST(NotEqualOperator, Equal) {
    Point p1 = Point(100.0, 200.0, 0);
    Point p2 = Point(100.0, 200.0, 0);
    
    EXPECT_FALSE(p1 != p2);
}
