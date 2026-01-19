/* 
    Author: Hanna Biegacz, updated: Maciej Cieślik
    
    Implementation of Point struct for lawn mower simulator. 
*/

#include <cmath>
#include <iostream>
#include "Point.h"
#include "Constants.h"

using namespace std;


Point::Point(const double& x, const double& y, const unsigned int& id) : x_(x), y_(y), id_(id) {}


Point::Point(const Point& other) : x_(other.x_), y_(other.y_), id_(other.id_) {}


Point& Point::operator=(const Point& other) {
    if (this != &other) {
        x_ = other.x_;
        y_ = other.y_;
        id_ = other.id_;
    }
    return *this;
}


double Point::getX() const {
    return x_;
}


double Point::getY() const {
    return y_;
}


unsigned int Point::getId() const {
    return id_;
}


bool Point::operator==(const Point& other) const {    
    bool x_equal = fabs(x_ - other.x_) < Constants::DISTANCE_PRECISION;
    bool y_equal = fabs(y_ - other.y_) < Constants::DISTANCE_PRECISION;
    bool id_equal = id_ == other.getId();

    return x_equal && y_equal && id_equal;
}


bool Point::operator!=(const Point& other) const {
    return !((*this) == other);
}


ostream& operator<<(ostream& os, const Point& point) {
    os << "Point(id: " << point.id_  << ", x: " << point.x_ << ", y: " << point.y_ << ")";
    return os;
}
