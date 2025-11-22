#ifndef POINT_H
#define POINT_H

#include <ostream>

struct Point {
    private:
        double x_;         
        double y_;         
        unsigned int id_;  
        static unsigned int idCounter_;  

    public:
        Point();
        Point(const double& x, const double& y);
        
        double getX() const;
        double getY() const;
        unsigned int getId() const;
        
        void setPosition(const double& x, const double& y);
        
        double calcDistanceTo(const Point& other) const;
        
        bool operator==(const Point& other) const;
        friend std::ostream& operator<<(std::ostream& os, const Point& point);
};

#endif // POINT_H
