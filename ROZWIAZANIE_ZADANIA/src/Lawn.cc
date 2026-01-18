/* 
    Author: Maciej Cieslik
    
    Describes Lawn, on which mower is cutting grass.
*/

#include <cmath>
#include <cstdint>
#include "Lawn.h"
#include "MathHelper.h"
#include "Config.h"

using namespace std;


Lawn::Lawn(const unsigned int& lawn_width, const unsigned int& lawn_length)
    : width_(lawn_width), length_(lawn_length)
    {
        Config::initializeRuntimeConstants(width_, length_);
        fields_.resize(Config::VERTICAL_FIELDS_NUMBER, std::vector<bool>(Config::HORIZONTAL_FIELDS_NUMBER, false));
    }


bool Lawn::operator==(const Lawn& other) const {
    return this->width_ == other.getWidth() && this->length_ == other.getLength() && 
        this->fields_ == other.getFields();
}


bool Lawn::operator!=(const Lawn& other) const {
    return !((*this) == other);
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
    // Check if point (x, y) is located inside the lawn.

    bool is_point_in_lawn_vertical = Lawn::countIfCoordInSection(length_, y);
    bool is_point_in_lawn_horizontal = Lawn::countIfCoordInSection(width_, x);

    return is_point_in_lawn_vertical && is_point_in_lawn_horizontal;
}


bool Lawn::countIfCoordInSection(const unsigned int& section_length,
        const double& coord_value) {
    // Check if single coord value is between the beggining and the end of section
    
    return coord_value >= 0.0 && coord_value <= static_cast<double>(section_length);
}


pair<unsigned int, unsigned int> Lawn::calculateFieldIndexes(const double& x, const double& y) const {
    // Calculate index of the field located inside the lawn

    unsigned int x_index = Lawn::calculateIndexInSection(width_, x, fields_[0].size());
    unsigned int y_index = Lawn::calculateIndexInSection(length_, y, fields_.size());

    pair<unsigned int, unsigned int> field_indexes = pair<unsigned int, unsigned int>(x_index, y_index);

    return field_indexes;
}


unsigned int Lawn::calculateIndexInSection(const unsigned int& section_length, const double& coord_value, 
        const unsigned int& vector_size) {
    // Calculate index of the coord in section

    unsigned int index = static_cast<unsigned int>(coord_value / Config::FIELD_WIDTH);

    return index;
}


void Lawn::cutGrassOnField(const pair<unsigned int, unsigned int>& indexes) {
    // Change field state to mowed 

    fields_[indexes.second][indexes.first] = true;
}


double Lawn::calculateShavedArea() const {
    // Calculate shaved area of the field as ratio of mowed fields to not mowed fields

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
    /* Cut grass in circle shape. Iterates over all fields of minimal square in which all circle can be fit.
        Each field is checked if it is in circle. If field is both in the circle and in the lawn as well, the field
        is mowed. Otherwise the field is not mowed. */

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
    // Calculate coords of the most left down field for mowing
    
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
    /* Check if field is in the range of mower's blade. Filed is mowed if it has 3 corners in range of blade or
        it has 2 corners in range of mower's blade and also the middle of the field is in range */

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
    // Calculates how many corners are in area accessible for blade

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
    // Calculate distance between two points

    double dx = x - destination_point.first;
    double dy = y - destination_point.second;

    return std::sqrt(dx * dx + dy * dy);
}


void Lawn::cutGrassSection(const std::pair<double, double>& blade_middle_beginning, const unsigned int& blade_diameter,
    const std::pair<double, double>& blade_middle_ending, const unsigned short& angle) {
    // Cuts grass area, which contsists of two circles and one rectangle
 
    cutGrass(blade_middle_beginning, blade_diameter);
    cutRectangularGrass(blade_middle_beginning, blade_diameter, blade_middle_ending, angle);
    cutGrass(blade_middle_ending, blade_diameter);
}


void Lawn::cutRectangularGrass(const std::pair<double, double>& blade_middle_beginning, 
    const unsigned int& blade_diameter, const std::pair<double, double>& blade_middle_ending, 
    const unsigned short& angle) {
    // Cuts rectangular shaped grass. Provides correct mowing mode, depends on the position of rectangle

    
    if (angle % 90 == 0) {
        cutVerticalRectangle(blade_middle_beginning, blade_diameter, blade_middle_ending);
    }
    else {
        cutTiltedRectangle(blade_middle_beginning, blade_diameter, blade_middle_ending, angle);
    }
}


void Lawn::cutTiltedRectangle(const std::pair<double, double>& blade_middle_beginning, 
    const unsigned int& blade_diameter, const std::pair<double, double>& blade_middle_ending, 
    const unsigned short& angle) {
    /* Cuts grass in tilted rectangular shape. Iterates through fields which are located inside the big 
    not tilted rectangle, which is the minimal rectangle, in which mowing rectangular area can be fit. 
    If field is both in big rectangle and small rectangle it is mowed.*/ 

    double angle_in_radians = MathHelper::convertDegreesToRadians(angle);
    double a_mower_path = MathHelper::calculateAParameter(angle);
    double a_perpendicular = MathHelper::calculateAPerpendicularParameter(a_mower_path);

    double DIAMETER_TO_RADIUS_FACTOR = 2;
    double blade_radius = blade_diameter / DIAMETER_TO_RADIUS_FACTOR;

    double x_left_beginning = blade_middle_beginning.first - cos(angle_in_radians) * blade_radius;
    double x_right_beginning = blade_middle_beginning.first + cos(angle_in_radians) * blade_radius;
    double x_left_ending = blade_middle_ending.first - cos(angle_in_radians) * blade_radius;
    double x_right_ending = blade_middle_ending.first + cos(angle_in_radians) * blade_radius;

    double b_horizontal_beginning = blade_middle_beginning.second - a_perpendicular * blade_middle_beginning.first;
    double b_horizontal_ending = blade_middle_ending.second - a_perpendicular * blade_middle_ending.first;

    double y_left_beginning = a_perpendicular * x_left_beginning + b_horizontal_beginning;
    double y_right_beginning = a_perpendicular * x_right_beginning + b_horizontal_beginning;
    double y_left_ending = a_perpendicular * x_left_ending + b_horizontal_ending;
    double y_right_ending = a_perpendicular * x_right_ending + b_horizontal_ending;

    double b_vertical_left = y_left_beginning - a_mower_path * x_left_beginning;
    double b_vertical_right = y_right_beginning - a_mower_path * x_right_beginning;

    double max_b_horizontal = max(b_horizontal_beginning, b_horizontal_ending);
    double min_b_horizontal = min(b_horizontal_beginning, b_horizontal_ending);
    double max_b_vertical = max(b_vertical_left, b_vertical_right);
    double min_b_vertical = min(b_vertical_left, b_vertical_right);
       
    double left_side_x = min(x_left_beginning, x_left_ending);
    double right_side_x = max(x_right_beginning, x_right_ending);

    double up_side_y = max(max(y_left_beginning, y_right_beginning), max(y_left_ending, y_right_ending));
    double down_side_y = min(min(y_left_beginning, y_right_beginning), min(y_left_ending, y_right_ending));

    pair<double, double> addition_factors = calculateAdditionFactors(angle);     

    double start_y = (addition_factors.second > 0) ? down_side_y : up_side_y;
    double end_y   = (addition_factors.second > 0) ? up_side_y : down_side_y;
    double start_x = (addition_factors.first > 0) ? left_side_x : right_side_x;
    double end_x   = (addition_factors.first > 0) ? right_side_x : left_side_x;

    double current_y = start_y;
    while ((addition_factors.second > 0 && current_y <= end_y) ||
        (addition_factors.second < 0 && current_y >= end_y)) {
        double current_x = start_x;

        while ((addition_factors.first > 0 && current_x <= end_x) ||
            (addition_factors.first < 0 && current_x >= end_x)) {
            double b_horizontal = current_y - a_perpendicular * current_x;
            double b_vertical = current_y - a_mower_path * current_x;

            if (min_b_horizontal <= b_horizontal && b_horizontal <= max_b_horizontal &&
                min_b_vertical <= b_vertical && b_vertical <= max_b_vertical &&
                isPointInLawn(current_x, current_y)) {

                pair<unsigned int, unsigned int> indexes = calculateFieldIndexes(current_x, current_y);
                cutGrassOnField(indexes); 
            }
            current_x += addition_factors.first;
        }
        current_y += addition_factors.second;
    }
}


pair<double, double> Lawn::calculateAdditionFactors(const unsigned short& angle) {
    // Calculate addition factor for fields for iteration in nested loop, depending on angle.

    double x_addition_factor = Config::FIELD_WIDTH;
    double y_addition_factor = Config::FIELD_WIDTH;

    if (angle > 90 && angle < 180) {
        y_addition_factor *= -1;
    }
    else if (angle > 180 && angle < 270) {
        x_addition_factor *= -1;
        y_addition_factor *= -1;
    }
    else if (angle > 270 && angle < 360){
        x_addition_factor *= -1;
    }

    return pair<double, double> (x_addition_factor, y_addition_factor);
}


void Lawn::cutVerticalRectangle(const std::pair<double, double>& blade_middle_beginning, 
    const unsigned int& blade_diameter, const std::pair<double, double>& blade_middle_ending) {
    // Cut grass in not tilted rectangular shape. Simplified version of default tilted rectangle mowing 

    double DIAMETER_TO_RADIUS_FACTOR = 2;
    double blade_radius = blade_diameter / DIAMETER_TO_RADIUS_FACTOR;

    double beginning_x = blade_middle_beginning.first;
    double beginning_y = blade_middle_beginning.second;
    double ending_x = blade_middle_ending.first;
    double ending_y = blade_middle_ending.second;

    double left_side_x;
    double down_side_y;
    double right_side_x;
    double up_side_y;
    if (beginning_x - ending_x == 0) {
        left_side_x = max(beginning_x - blade_radius, 0.0);
        down_side_y = max(min(beginning_y, ending_y), 0.0);
        right_side_x = min(double(width_), beginning_x + blade_radius);
        up_side_y = min(max(beginning_y, ending_y), double(length_));
    }
    else {
        left_side_x = max(min(beginning_x, ending_x), 0.0);
        down_side_y = max(beginning_y - blade_radius, 0.0);
        right_side_x = min(max(beginning_x, ending_x), double(width_));
        up_side_y = min(beginning_y + blade_radius, double(length_));
    }

    pair<unsigned int, unsigned int> indexes = calculateFieldIndexes(left_side_x, down_side_y);
    beginning_x = indexes.first * Config::FIELD_WIDTH + Config::FIELD_WIDTH / 2;
    beginning_y = indexes.second * Config::FIELD_WIDTH + Config::FIELD_WIDTH / 2;

    for (double current_y = beginning_y; current_y <= up_side_y; current_y += Config::FIELD_WIDTH) {
        for (double current_x = beginning_x; current_x <= right_side_x; current_x += Config::FIELD_WIDTH) {
            pair<unsigned int, unsigned int> indexes = calculateFieldIndexes(current_x, current_y);
            cutGrassOnField(indexes);
        }
    }
}
