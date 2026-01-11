/* 
    Author: Maciej Cieslik
    
    Implements MathHelper class.
*/

#include <cmath>
#include "../include/Constants.h"
#include "../include/MathHelper.h"

using namespace std;


double MathHelper::convertDegreesToRadians(const unsigned short& angle) {
    double RADIAN_FACTOR = 180.0;
    const double DEGREE_TO_RADIAN_FACTOR = Constants::PI / RADIAN_FACTOR;
    double radians = angle * DEGREE_TO_RADIAN_FACTOR;
    return radians;
}


double MathHelper::convertRadiansToDegrees(const double& angle) {
    double DEGREES_FACTOR = 180.0;
    return angle * DEGREES_FACTOR / Constants::PI;
}


double MathHelper::calculateAParameter(const unsigned short& angle) {
    double angle_in_radians = MathHelper::convertDegreesToRadians(angle);
    return tan(angle_in_radians);
    
}


double MathHelper::calculateAPerpendicularParameter(const double& a_parameter) {
    return -1.0 / a_parameter;
}


double MathHelper::roundNumber(const double& value, const double& precision) {
    return round(value * precision) / precision;
}
