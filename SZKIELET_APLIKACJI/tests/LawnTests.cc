/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include "../include/Lawn.h"
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
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 100000.0;
    double y = 100000.0;

    bool result = lawn.isPointInLawn(x, y);

    EXPECT_EQ(true, result);
}


TEST(IsPointInLawn, isPointInLawnIncorrectSquareLawn) {
    unsigned int lawn_width = 60000;
    unsigned int lawn_length = 60000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 60000.1;
    double y = 50000.1;

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
    double x = 100.0;
    double y = 100.0;
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
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 100000.0;
    double y = 100000.0;
    pair<unsigned int, unsigned int> pattern (999, 999);

    pair<unsigned int, unsigned int> result = lawn.calculateFieldIndexes(x, y);

    EXPECT_EQ(pattern, result);
}


TEST(CalculateFieldIndexes, calculateFieldIndexesRightUpCornerMaxRatioLawn) {
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 10000;
    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Lawn lawn = Lawn(lawn_width, lawn_length);
    double x = 100000.0;
    double y = 10000.0;
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
    pair<unsigned int, unsigned int> pattern (499, 499);

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
