/* 
    Author: Maciej Cieslik
    
    Describes mover. Mover has rectangular shape with blade, which middle is located in the central point of the mover.
    Blade cuts grass in circular area. The mover moves in continuous space(mover can cover the part of the field).
    Location of mover is described by coordinates(x, y) of it's middle point.
*/

class Mover {
private:
    unsigned int width_; // cm
    unsigned int length_; // cm
    unsigned int blade_diameter_; // cm
    unsigned int speed_; // cm/s
    unsigned short angle_; // (0-359) degree
    double x_;
    double y_;

    std::pair<double, double> calculateFinalPoint(const double& distance) const;
    double convertDegreesToRadians() const;
    bool calculateIfXAccessible(const double& calculatedX, const unsigned int& lawn_width) const;
    bool calculateIfYAccessible(const double& calculatedY, const unsigned int& lawn_length) const;

public:
    Mover(const unsigned int& width, const unsigned int& length, const unsigned int& blade_diameter,
        const unsigned int& speed);
    Mover(const Mover&) = delete;
    Mover& operator=(const Mover&) = delete;
    
    unsigned int getWidth() const;
    unsigned int getLength() const;
    unsigned int getBladeDiameter() const;
    unsigned int getSpeed() const;
    unsigned short getAngle() const;
    double getX() const;
    double getY() const;

    void setAngle(const unsigned short& new_angle);
    void setX(const double& new_x);
    void setY(const double& new_y);

    void move(const double& distance, const unsigned int& lawn_width, const unsigned int& lawn_length);
    void rotate(const short& angle);
};