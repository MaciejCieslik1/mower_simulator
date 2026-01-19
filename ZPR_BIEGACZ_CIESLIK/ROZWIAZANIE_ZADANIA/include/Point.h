/*
    Author: Hanna Biegacz, updated: Maciej Cieślik

    Represents a marker point placed by user on the lawn. 
    Point consists of x and y coordinates (double values) and unique identifier (unsigned int). 
    Each Point is assigned auto-incremented ID via static id_counter to ensure uniqueness. 
*/

#pragma once
#include <ostream>


struct Point {
    private:
        double x_;         
        double y_;         
        unsigned int id_;  

    public:
        Point(const double& x, const double& y, const unsigned int& id);
        Point(const Point& other);
        Point& operator=(const Point&);
        
        double getX() const;
        double getY() const;
        unsigned int getId() const;
                
        bool operator==(const Point& another) const;
        bool operator!=(const Point& another) const;
        friend std::ostream& operator<<(std::ostream& os, const Point& point);
};
