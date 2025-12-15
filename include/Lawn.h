/* 
    Author: Maciej Cieslik
    
    Describes Lawn, on which mower is cutting grass. Lawn consists of fields, which are repesented by 
    bool element in 2-dimensional vector. False meaning the grass is not cut, true meaning the grass is cut. 
    Left down corner point has coordinates (0.0, 0.0).
*/
#pragma once
#include <vector>

class Lawn {
private:
    unsigned int width_;
    unsigned int length_;
    // Outer vector represents length(vertical), inner represents width(horizontal)
    std::vector<std::vector<bool>> fields_; 

    bool isFieldInMowingArea(const double& x, const double& y, const std::pair<double, double>& blade_middle, 
        const double& blade_diameter) const;
    double calculateDistanceBetweenPoints(const double& x, const double& y, 
        const std::pair<double, double>& destination_point) const;
    unsigned int countCornersInArea(const double& x, const double& y, const std::pair<double, double>& blade_middle, 
        const double& blade_diameter) const;
    std::pair<double, double> calculateFirstMowingFieldCoords(const std::pair<double, double>& blade_middle, 
        const double& blade_diameter) const;

public:
    Lawn(const unsigned int& lawn_width, const unsigned int& lawn_length);
    Lawn(const Lawn&) = delete;
    Lawn& operator=(const Lawn&) = delete;

    unsigned int getWidth() const;
    unsigned int getLength() const;
    std::vector<std::vector<bool>> getFields() const;

    bool isPointInLawn(const double& x, const double& y) const;
    std::pair<unsigned int, unsigned int> calculateFieldIndexes(const double& x, const double& y) const;
    void cutGrassOnField(const std::pair<unsigned int, unsigned int>& indexes);

    static bool countIfCoordInSection(const unsigned int& section_length, const double& coord_value);
    static unsigned int calculateIndexInSection(const unsigned int& section_length, const double& coord_value, 
        const unsigned int& vector_size);
    
    double calculateShavedArea() const;
    void cutGrass(const std::pair<double, double>& blade_middle, const unsigned int& blade_diameter);
};
