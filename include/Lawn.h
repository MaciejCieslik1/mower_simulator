/* 
    Author: Maciej Cieslik
    
    Describes Lawn, on which mover is cutting grass. Lawn consists of fields, which are repesented by 
    bool element in 2-dimensional vector. False meaning the grass is not cut, true meaning the grass is cut. 
    Left down corner point has coordinates (0.0, 0.0).
*/

#include <vector>

class Lawn {
private:
    unsigned int width_;
    unsigned int length_;
    // Outer vector represents length(vertical), inner represents width(horizontal)
    std::vector<std::vector<bool>> fields_; 

public:
    Lawn(const unsigned int& lawn_width, const unsigned int& lawn_length);
    Lawn(const Lawn&) = delete;
    Lawn& operator=(const Lawn&) = delete;

    unsigned int getWidth();
    unsigned int getLength();

    bool isPointInLawn(const double& x, const double& y);
    static bool countIfCoordInSection(const unsigned int& section_length, const double& coord_value);
};
