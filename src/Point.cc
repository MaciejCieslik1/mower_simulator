/* 
    Author: Hanna Biegacz
    
    Implementation of Point struct for lawn mower simulator. 
*/

#include "../include/Point.h"
#include <cmath>
#include <iostream>

using namespace std;


unsigned int Point::idCounter_ = 0;


Point::Point() 
    : x_(0.0), y_(0.0), id_(idCounter_++) {}


Point::Point(const double& x, const double& y)
    : x_(x), y_(y), id_(idCounter_++) {}


double Point::getX() const {
    return x_;
}

double Point::getY() const {
    return y_;
}

unsigned int Point::getId() const {
    return id_;
}

void Point::setPosition(const double& x, const double& y) {
    x_ = x;
    y_ = y;
}

double Point::calcDistanceTo(const Point& other) const {
    /* Pythagorean theorem */ 
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    
    return sqrt(dx * dx + dy * dy);
}

bool Point::operator==(const Point& other) const {
    const double EPSILON = 1e-9;     
    bool x_equal = fabs(x_ - other.x_) < EPSILON;
    bool y_equal = fabs(y_ - other.y_) < EPSILON;
    
    return x_equal && y_equal;
}


ostream& operator<<(ostream& os, const Point& point) {
    os << "Point(id: " << point.id_  << ", x: " << point.x_ << ", y: " << point.y_ << ")";
    return os;
}
