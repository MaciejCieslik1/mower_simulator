/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include <cmath>
#include <cstdint>
#include "../include/Lawn.h"
#include "../include/Constants.h"
#include "../include/Config.h"

using namespace std;


TEST(Getters, Getters) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    std::vector<std::vector<bool>> lawn_fields (Config::VERTICAL_FIELDS_NUMBER, 
        std::vector<bool>(Config::HORIZONTAL_FIELDS_NUMBER, false));
    Lawn lawn = Lawn(lawn_width, lawn_length);

    unsigned int width = lawn.getWidth();
    unsigned int length = lawn.getWidth();
    std::vector<std::vector<bool>> fields = lawn.getFields();

    EXPECT_EQ(width, lawn_width);
    EXPECT_EQ(length, lawn_length);
    EXPECT_EQ(fields, lawn_fields);
}


TEST(OperatorEquals, equals) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width, lawn_length);

    bool result = lawn == lawn2;
    EXPECT_TRUE(result);
}


TEST(OperatorEquals, notEqualsLawnWidth) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width + 1, lawn_length);

    bool result = lawn == lawn2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsLawnLength) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width, lawn_length + 1);

    bool result = lawn == lawn2;
    EXPECT_FALSE(result);
}


TEST(OperatorEquals, notEqualsFields) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width, lawn_length);
    lawn2.cutGrassOnField(std::pair<unsigned int, unsigned int>(1, 1));

    bool result = lawn == lawn2;
    EXPECT_FALSE(result);
}


TEST(OperatorNotEquals, OperatorEquals_notEqualsLawnWidth_Test) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width + 1, lawn_length);

    bool result = lawn != lawn2;
    EXPECT_TRUE(result);
}


TEST(OperatorNotEquals, equals) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    Lawn lawn2 = Lawn(lawn_width, lawn_length);

    bool result = lawn != lawn2;
    EXPECT_FALSE(result);
}


TEST(IsPointInLawn, isPointInLawnCorrect) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 0.5;
    double y = 0.3;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(true, result);
}

TEST(IsPointInLawn, isPointInLawnCorrectMinimalValues) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 0.0;
    double y = 0.0;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(true, result);
}


TEST(IsPointInLawn, isPointInLawnCorrectMaximalValues) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 10000.0;
    double y = 10000.0;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(true, result);
}


TEST(IsPointInLawn, isPointInLawnIncorrectSquareLawn) {
    unsigned int lawn_width = 6000;
    unsigned int lawn_length = 6000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 6000.1;
    double y = 5000.1;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(false, result);
}


TEST(IsPointInLawn, isPointInLawnIncorrectRectangularLawn) {
    unsigned int lawn_width = 60000;
    unsigned int lawn_length = 6000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 50000.1;
    double y = 6000.1;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(false, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesLeftDownCornerMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 0.0;
    double y = 0.0;
    pair<unsigned int, unsigned int> pattern (0, 0);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);
    

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesRightUpCornerMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 99.999;
    double y = 99.999;
    pair<unsigned int, unsigned int> pattern (999, 999);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesLeftDownCornerMaximalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 0.0;
    double y = 0.0;
    pair<unsigned int, unsigned int> pattern (0, 0);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);
    

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesRightUpCornerMaximalLawn) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 9999.999;
    double y = 9999.999;
    pair<unsigned int, unsigned int> pattern (999, 999);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesRightUpCornerMaxRatioLawn) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 9999.999;
    double y = 999.999;
    pair<unsigned int, unsigned int> pattern (9999, 999);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesMiddleMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 50.0;
    double y = 50.0;
    pair<unsigned int, unsigned int> pattern (500, 500);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesCustomValuesCustomLawnMaxRatio) {
    unsigned int lawn_width = 60000;
    unsigned int lawn_length = 6000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 15789.2;
    double y = 5799.8;
    pair<unsigned int, unsigned int> pattern (2631, 966);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesCustomValuesCustomLawnMaxRatio2) {
    unsigned int lawn_width = 60010;
    unsigned int lawn_length = 6010;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 15782.2;
    double y = 5792.8;
    pair<unsigned int, unsigned int> pattern (2625, 963);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesCustomValuesCustomLawnCustomRatio) {
    unsigned int lawn_width = 41210;
    unsigned int lawn_length = 6410;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 15782.2;
    double y = 5792.8;
    pair<unsigned int, unsigned int> pattern (2462, 903);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CutGrassOnFields, cutGrassOnField) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<unsigned int, unsigned int> indexes (151, 3);

    lawn.cutGrassOnField(indexes);

    EXPECT_EQ(true, lawn.getFields()[indexes.second][indexes.first]);
}


TEST(CalculateShavedArea, calculateShavedAreaCustom) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<unsigned int, unsigned int> indexes1 (151, 3);
    pair<unsigned int, unsigned int> indexes2 (152, 3);
    pair<unsigned int, unsigned int> indexes3 (153, 3);
    pair<unsigned int, unsigned int> indexes4 (154, 3);
    pair<unsigned int, unsigned int> indexes5 (155, 3);
    pair<unsigned int, unsigned int> indexes6 (156, 3);
    pair<unsigned int, unsigned int> indexes7 (157, 3);
    pair<unsigned int, unsigned int> indexes8 (158, 3);
    pair<unsigned int, unsigned int> indexes9 (159, 3);
    pair<unsigned int, unsigned int> indexes0 (160, 3);
    double shavedFactor = 0.00001;

    lawn.cutGrassOnField(indexes1);
    lawn.cutGrassOnField(indexes2);
    lawn.cutGrassOnField(indexes3);
    lawn.cutGrassOnField(indexes4);
    lawn.cutGrassOnField(indexes5);
    lawn.cutGrassOnField(indexes6);
    lawn.cutGrassOnField(indexes7);
    lawn.cutGrassOnField(indexes8);
    lawn.cutGrassOnField(indexes9);
    lawn.cutGrassOnField(indexes0);

    EXPECT_EQ(shavedFactor, lawn.calculateShavedArea());
}


TEST(CalculateShavedArea, calculateShavedAreaNotShaved) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double shavedFactor = 0.0;

    EXPECT_EQ(shavedFactor, lawn.calculateShavedArea());
}


TEST(CutGrass, cutGrassFullCircleIntBladeMiddleMinLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (50, 50);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;
    double max_radius = blade_diameter / 2 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2 - Config::FIELD_WIDTH; 
    double min_area = Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassFullCircleMaxLawn) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (50, 50);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnDownSide) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (50, 0);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.5 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnUpSide) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (50, 10000);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.5 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);

    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnLeftSide) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (0, 50);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.5 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnRigthSide) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (10000, 50);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.5 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnLeftDownCorner) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (0, 0);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.25 * Constants::PI * min_radius * min_radius;
    
    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnRightDownCorner) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (10000, 0);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.25 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassLawnRightUpCorner) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (10000, 10000);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.25 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(CutGrass, cutGrassMaxLawnLeftUpCorner) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (0, 10000);
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;
    double max_radius = blade_diameter / 2.0 + Config::FIELD_WIDTH; 
    double max_area = Constants::PI * max_radius * max_radius;
    double min_radius = blade_diameter / 2.0 - Config::FIELD_WIDTH; 
    double min_area = 0.25 * Constants::PI * min_radius * min_radius;

    lawn.cutGrass(blade_middle, blade_diameter);
    int64_t lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    
    EXPECT_LT(shaved_area, max_area);
    EXPECT_GT(shaved_area, min_area);
}


TEST(cutGrassSection, cutTitledAreaAllInside) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (250, 250);
    pair<double, double> ending_point (750, 750);
    unsigned short angle = 45;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500 * sqrt(2);
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}


TEST(cutGrassSection, cutTitledAreaAllInsideAngle225) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (750, 750);
    pair<double, double> ending_point (250, 250);
    unsigned short angle = 225;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500 * sqrt(2);
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}


TEST(cutGrassSection, cutNormalAreaAllInside) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (250, 250);
    pair<double, double> ending_point (250, 750);
    unsigned short angle = 0;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500;
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}


TEST(cutGrassSection, cutNormalAreaAllInside2) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (250, 250);
    pair<double, double> ending_point (750, 250);
    unsigned short angle = 90;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500;
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}

TEST(cutGrassSection, cutNormalAreaAllInside3) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (250, 750);
    pair<double, double> ending_point (250, 250);
    unsigned short angle = 180;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500;
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}


TEST(cutGrassSection, cutNormalAreaAllInside4) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 1000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    pair<double, double> blade_middle (750, 250);
    pair<double, double> ending_point (250, 250);
    unsigned short angle = 270;
    unsigned int blade_diameter = Config::MIN_BLADE_DIAMETER;;

    lawn.cutGrassSection(blade_middle, blade_diameter, ending_point, angle);
    unsigned int lawn_area = lawn_width * lawn_length;
    double shaved_area = lawn.calculateShavedArea() * static_cast<double>(lawn_area);
    double estimated_shaved_area = Constants::PI * blade_diameter * blade_diameter / 4 + blade_diameter * 500;
    
    EXPECT_NEAR(shaved_area, estimated_shaved_area, 0.1 * estimated_shaved_area);
}


