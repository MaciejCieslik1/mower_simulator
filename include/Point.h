/*
    Author: Hanna Biegacz

    Represents a marker point placed by user on the lawn. 
    Point consists of x and y coordinates (double values) and unique identifier (unsigned int). 
    Each Point is assigned auto-incremented ID via static id_counter to ensure uniqueness. 
*/

#ifndef POINT_H
#define POINT_H

#include <ostream>

struct Point {
    private:
        double x_;         
        double y_;         
        unsigned int id_;  
        static unsigned int id_counter_;  

    public:
        Point(const double& x, const double& y);
        Point(const Point&) = delete;
        Point& operator=(const Point&) = delete;
        
        double getX() const;
        double getY() const;
        unsigned int getId() const;
                
        double calcDistanceTo(const Point& another) const;
        
        bool operator==(const Point& another) const;
        friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif // POINT_H
