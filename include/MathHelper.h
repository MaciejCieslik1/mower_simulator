/* 
    Author: Maciej Cieslik
    
    Class which provides mathematical operations as static methods.
*/

class MathHelper {
public:
    static double convertDegreesToRadians(const unsigned short& angle);
    static double calculateAParameter(const unsigned short& angle);
    static double calculateAPerpendicularParameter(const double& a_parameter);
    static double roundNumber(const double& value, const double& precision);
};