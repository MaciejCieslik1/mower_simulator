/* 
    Author: Maciej Cieslik
    
    Describes Lawn, on which mover is cutting grass.
*/

#include <iostream>
#include <cmath>
#include "../include/Lawn.h"
#include "../include/Config.h"

using namespace std;


Lawn::Lawn(const unsigned int& lawn_width, const unsigned int& lawn_length)
    : width_(lawn_width), length_(lawn_length), fields_(lawn_length, std::vector<bool>(lawn_width, false)) {}


unsigned int Lawn::getWidth() {
    return width_;
}


unsigned int Lawn::getLength(){
    return length_;
}


bool Lawn::isPointInLawn(const double& x, const double& y) {
    // check if point is located inside the lawn.

    unsigned int vertical_points_number = fields_.size();
    unsigned int horizontal_points_number = fields_[0].size();
    
    bool is_point_in_lawn_vertical = Lawn::countIfCoordInSection(length_, y);
    bool is_point_in_lawn_horizontal = Lawn::countIfCoordInSection(width_, x);

    return is_point_in_lawn_vertical && is_point_in_lawn_horizontal;
}


bool Lawn::countIfCoordInSection(const unsigned int& section_length,
        const double& coord_value) {
    return coord_value <= static_cast<double>(section_length);
}


pair<unsigned int, unsigned int> Lawn::calculateFieldIndexes(const double& x, const double& y) {
    unsigned int x_index = Lawn::calculateIndexInSection(width_, x, fields_[0].size());
    unsigned int y_index = Lawn::calculateIndexInSection(length_, y, fields_.size());

    pair<unsigned int, unsigned int> field_indexes = pair<unsigned int, unsigned int>(x_index, y_index);

    return field_indexes;
}


unsigned int Lawn::calculateIndexInSection(const unsigned int& section_length, const double& coord_value, 
        const unsigned int& vector_size) {
    unsigned int INDEX_OFFSET = 1;
    
    unsigned int index = static_cast<unsigned int>(ceil(coord_value / Config::FIELD_WIDTH));
    if (index != 0) {
        index -= INDEX_OFFSET;
    }

    return index;
}
