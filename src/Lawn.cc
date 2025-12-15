/* 
    Author: Maciej Cieslik
    
    Describes Lawn, on which mower is cutting grass.
*/

#include <iostream>
#include <cmath>
#include <cstdint>
#include "../include/Lawn.h"
#include "../include/Config.h"

using namespace std;


Lawn::Lawn(const unsigned int& lawn_width, const unsigned int& lawn_length)
    : width_(lawn_width), length_(lawn_length)
    {
        Config::initializeRuntimeConstants(width_, length_);
        fields_.resize(Config::VERTICAL_FIELDS_NUMBER, std::vector<bool>(Config::HORIZONTAL_FIELDS_NUMBER, false));
    }

unsigned int Lawn::getWidth() const {
    return width_;
}


unsigned int Lawn::getLength() const {
    return length_;
}


std::vector<std::vector<bool>> Lawn::getFields() const {
    return fields_;
}


bool Lawn::isPointInLawn(const double& x, const double& y) const {
    bool is_point_in_lawn_vertical = Lawn::countIfCoordInSection(length_, y);
    bool is_point_in_lawn_horizontal = Lawn::countIfCoordInSection(width_, x);

    return is_point_in_lawn_vertical && is_point_in_lawn_horizontal;
}


bool Lawn::countIfCoordInSection(const unsigned int& section_length,
        const double& coord_value) {
    return coord_value <= static_cast<double>(section_length);
}


pair<unsigned int, unsigned int> Lawn::calculateFieldIndexes(const double& x, const double& y) const {
    unsigned int x_index = Lawn::calculateIndexInSection(width_, x, fields_[0].size());
    unsigned int y_index = Lawn::calculateIndexInSection(length_, y, fields_.size());

    pair<unsigned int, unsigned int> field_indexes = pair<unsigned int, unsigned int>(x_index, y_index);

    return field_indexes;
}


unsigned int Lawn::calculateIndexInSection(const unsigned int& section_length, const double& coord_value, 
        const unsigned int& vector_size) {
    unsigned int index = static_cast<unsigned int>(coord_value / Config::FIELD_WIDTH);

    return index;
}


void Lawn::cutGrassOnField(const pair<unsigned int, unsigned int>& indexes) {
    fields_[indexes.second][indexes.first] = true;
}


double Lawn::calculateShavedArea() const {
    int64_t all_fields_number = static_cast<int64_t>(Config::HORIZONTAL_FIELDS_NUMBER) * 
        static_cast<int64_t>(Config::VERTICAL_FIELDS_NUMBER);
    int64_t shaved_fields_number = 0;

    for (const std::vector<bool>& shavedRow : getFields()) {
        for (bool field : shavedRow) {
            if (field) shaved_fields_number ++;
        }
    }

    return static_cast<double>(shaved_fields_number) / static_cast<double>(all_fields_number);
}


void Lawn::cutGrass(const pair<double, double>& blade_middle, const unsigned int& blade_diameter) {
    double DIAMETER_TO_RADIUS_DIVISION_FACTOR = 2.0;

    pair<double, double> first_coords = calculateFirstMowingFieldCoords(blade_middle, blade_diameter);
    pair<unsigned int, unsigned int> first_indexes = calculateFieldIndexes(first_coords.first, first_coords.second);
    double beginning_x = static_cast<double>(first_indexes.first) * Config::FIELD_WIDTH;
    double beginning_y = static_cast<double>(first_indexes.second) * Config::FIELD_WIDTH;
    double ending_x = min(beginning_x + blade_diameter, static_cast<double>(width_));
    double ending_y = min(beginning_y + blade_diameter, static_cast<double>(length_));
    double current_y = beginning_y;
    double current_x = beginning_x;

    while (current_y < ending_y) {
        while (current_x < ending_x) {
            if (isFieldInMowingArea(current_x, current_y, blade_middle, blade_diameter)) {
                pair<unsigned int, unsigned int> indexes = calculateFieldIndexes(current_x, current_y);
                cutGrassOnField(indexes);
            }
            current_x = current_x + Config::FIELD_WIDTH;
        }
        current_y = current_y + Config::FIELD_WIDTH;
        current_x = beginning_x;
    }
}


pair<double, double> Lawn::calculateFirstMowingFieldCoords(const pair<double, double>& blade_middle, const double& blade_diameter) const {
    double DIAMETER_TO_RADIUS_DIVISION_FACTOR = 2.0;
    double LEFT_DOWN_CORNER_COORD = 0.0;
    double first_x = blade_middle.first - static_cast<double>(blade_diameter) / DIAMETER_TO_RADIUS_DIVISION_FACTOR;
    double first_y = blade_middle.second - static_cast<double>(blade_diameter) / DIAMETER_TO_RADIUS_DIVISION_FACTOR;
    
    pair<double, double> coords = pair<double, double>(max(first_x, LEFT_DOWN_CORNER_COORD), 
        max(first_y, LEFT_DOWN_CORNER_COORD));
    return coords;
}


bool Lawn::isFieldInMowingArea(const double& x, const double& y, const std::pair<double, double>& blade_middle, 
        const double& blade_diameter) const {
    unsigned int counter = countCornersInArea(x, y, blade_middle, blade_diameter);
    
    if (counter > 2) {
        return true;
    }
    else if (counter == 2) {
        return calculateDistanceBetweenPoints(x + Config::FIELD_WIDTH / 2.0, y + Config::FIELD_WIDTH / 2.0,
            blade_middle) <= blade_diameter / 2;
    }
    else {
        return false;
    }
}


unsigned int Lawn::countCornersInArea(const double& x, const double& y, const std::pair<double, double>& blade_middle, 
        const double& blade_diameter) const {
    pair<double, double> points[4] = {
        {x, y},
        {x + Config::FIELD_WIDTH, y},
        {x + Config::FIELD_WIDTH, y + Config::FIELD_WIDTH},
        {x, y + Config::FIELD_WIDTH}
    };

    unsigned int counter = 0;
    for (pair<double, double> point : points) {
        if (calculateDistanceBetweenPoints(point.first, point.second, blade_middle) <= blade_diameter / 2.0) {
            counter ++;
        }
    }

    return counter;
}


double Lawn::calculateDistanceBetweenPoints(const double& x, const double& y, 
    const std::pair<double, double>& destination_point) const {
    double dx = x - destination_point.first;
    double dy = y - destination_point.second;

    return std::sqrt(dx*dx + dy*dy);
}
