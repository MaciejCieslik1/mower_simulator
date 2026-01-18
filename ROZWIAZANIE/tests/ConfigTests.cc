/* 
    Author: Maciej Cieslik
    
    Tests initializeRuntimeConstants function in Config.cc file.
*/

#include <gtest/gtest.h>
#include "../include/Constants.h"
#include "../include/Config.h"

using namespace Config;


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterMinimalSizeLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 10u);
}


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterMaximalSizeLawn) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterCustomSizeLawn) {
    unsigned int lawn_width = 5000;
    unsigned int lawn_length = 6000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 50u);
}


TEST(InitializeRuntimeConstantsTest, MaxBladeDiameterMaximalSizeLawn) {
    unsigned int lawn_width = 10000;
    unsigned int lawn_length = 10000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MAX_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MaxBladeDiameterCustomSizeLawn) {
    unsigned int lawn_width = 5000;
    unsigned int lawn_length = 6000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MAX_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MowerSizes) {
    unsigned int lawn_width = 5000;
    unsigned int lawn_length = 6000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_MOWER_WIDTH, 50u);
    EXPECT_EQ(MAX_MOWER_WIDTH, 200u);
    EXPECT_EQ(MIN_MOWER_LENGTH, 50u);
    EXPECT_EQ(MAX_MOWER_LENGTH, 200u);
}


TEST(InitializeRuntimeConstantsTest, FieldWidthMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    double expected = std::min(lawn_width, lawn_length) / 1000.0;

    EXPECT_NEAR(FIELD_WIDTH, expected, 1e-9);
}


TEST(InitializeRuntimeConstantsTest, FieldWidthMaximalLawn) {
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    double expected = std::min(lawn_width, lawn_length) / 1000.0;

    EXPECT_NEAR(FIELD_WIDTH, expected, 1e-9);
}


TEST(InitializeRuntimeConstantsTest, FieldCustomLawn) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 60000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    double expected = std::min(lawn_width, lawn_length) / 1000.0;

    EXPECT_NEAR(FIELD_WIDTH, expected, 1e-9);
}


TEST(InitializeRuntimeConstantsTest, FieldCustomLawnMaxRation) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 5000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    double expected = std::min(lawn_width, lawn_length) / 1000.0;

    EXPECT_NEAR(FIELD_WIDTH, expected, 1e-9);
}


TEST(InitializeRuntimeConstantsTest, VerticalAndHorizontalFieldNumberMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(VERTICAL_FIELDS_NUMBER, 1000u);
    EXPECT_EQ(HORIZONTAL_FIELDS_NUMBER, 1000u);
}


TEST(InitializeRuntimeConstantsTest, VerticalAndHorizontalFieldNumberMaximalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(VERTICAL_FIELDS_NUMBER, 1000u);
    EXPECT_EQ(HORIZONTAL_FIELDS_NUMBER, 1000u);
}


TEST(InitializeRuntimeConstantsTest, VerticalAndHorizontalFieldNumberCustomLawn1) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 60000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(VERTICAL_FIELDS_NUMBER, 1200u);
    EXPECT_EQ(HORIZONTAL_FIELDS_NUMBER, 1000u);
}


TEST(InitializeRuntimeConstantsTest, VerticalAndHorizontalFieldNumberCustomLawn2) {
    unsigned int lawn_width = 5000;
    unsigned int lawn_length = 50000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(VERTICAL_FIELDS_NUMBER, 10000u);
    EXPECT_EQ(HORIZONTAL_FIELDS_NUMBER, 1000u);
}


TEST(InitializeRuntimeConstantsTest, SpeedLimitsMinimalLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_SPEED, 10);
    EXPECT_EQ(MAX_SPEED, 10);
}


TEST(InitializeRuntimeConstantsTest, SpeedLimitsMaximalLawn) {
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_SPEED, 100);
    EXPECT_EQ(MAX_SPEED, 1000);
}


TEST(InitializeRuntimeConstantsTest, SpeedLimitsCustomSmallLawn) {
    unsigned int lawn_width = 1000;
    unsigned int lawn_length = 10000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_SPEED, 10);
    EXPECT_EQ(MAX_SPEED, 100);
}


TEST(InitializeRuntimeConstantsTest, SpeedLimitsCustomMediumLawn) {
    unsigned int lawn_width = 5550;
    unsigned int lawn_length = 6320;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_SPEED, 10);
    EXPECT_EQ(MAX_SPEED, 555);
}


TEST(InitializeRuntimeConstantsTest, SpeedLimitsCustomBigLawn) {
    unsigned int lawn_width = 55500;
    unsigned int lawn_length = 63200;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_SPEED, 55);
    EXPECT_EQ(MAX_SPEED, 1000);
}


TEST(InitializeMowerConstatsTest, initializeMowerConstants) {
    unsigned int mower_width = 100;
    unsigned int mower_length = 90;
    unsigned short starting_angle = 10;
    double starting_x = 5.0;
    double starting_y = 3.0;

    initializeMowerConstants(mower_width, mower_length, starting_x, starting_y, starting_angle);

    EXPECT_NEAR(MAX_HORIZONTAL_EXCEEDANCE, Constants::DISTANCE_PRECISION, 1e-9);
    EXPECT_NEAR(MAX_VERTICAL_EXCEEDANCE, Constants::DISTANCE_PRECISION, 1e-9);
    EXPECT_EQ(STARTING_ANGLE, 10);
    EXPECT_EQ(STARTING_X, 5.0);
    EXPECT_EQ(STARTING_Y, 3.0);
}
