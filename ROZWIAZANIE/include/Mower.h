/* 
    Author: Maciej Cieslik
    
    Describes mower. Mower has rectangular shape with blade, which middle is located in the central point of the mower.
    Blade cuts grass in circular area. The mower moves in continuous space(mower can cover the part of the field).
    Location of mower is described by coordinates(x, y) of it's middle point.
*/

#pragma once


class Mower {
private:
    unsigned int width_; // cm
    unsigned int length_; // cm
    unsigned int blade_diameter_; // cm
    unsigned int speed_; // cm/s
    unsigned short angle_; // (0-359) degree
    bool is_mowing_;
    double x_;
    double y_;

    std::pair<double, double> calculateFinalPoint(const double& distance) const;
    bool calculateIfXAccessible(const double& calculatedX, const unsigned int& lawn_width) const;
    bool calculateIfYAccessible(const double& calculatedY, const unsigned int& lawn_length) const;

public:
    Mower(const unsigned int& width, const unsigned int& length, const unsigned int& blade_diameter,
        const unsigned int& speed);
    Mower(const Mower&) = delete;
    Mower& operator=(const Mower&) = delete;
    bool operator==(const Mower& other) const;
    bool operator!=(const Mower& other) const;
    
    unsigned int getWidth() const;
    unsigned int getLength() const;
    unsigned int getBladeDiameter() const;
    unsigned int getSpeed() const;
    unsigned short getAngle() const;
    bool getIsMowing() const;
    double getX() const;
    double getY() const;

    void setAngle(const unsigned short& new_angle);
    void setX(const double& new_x);
    void setY(const double& new_y);

    void move(const double& distance, const unsigned int& lawn_width, const unsigned int& lawn_length);
    void rotate(const short& angle);
    void turnOnMowing();
    void turnOffMowing();
};
