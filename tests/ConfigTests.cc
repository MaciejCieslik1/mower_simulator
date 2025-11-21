/* 
    Author: Maciej Cieslik
    
    Tests initializeRuntimeConstants function in Config.cc file.
*/

#include <gtest/gtest.h>
#include "../include/Config.h"

using namespace Config;


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterMinimalSizeLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 10u);
}


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterMaximalSizeLawn) {
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MinBladeDiameterCustomSizeLawn) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 60000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_BLADE_DIAMETER, 50u);
}


TEST(InitializeRuntimeConstantsTest, MaxBladeDiameterMaximalSizeLawn) {
    unsigned int lawn_width = 100000;
    unsigned int lawn_length = 100000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MAX_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MaxBladeDiameterCustomSizeLawn) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 60000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MAX_BLADE_DIAMETER, 100u);
}


TEST(InitializeRuntimeConstantsTest, MoverSizes) {
    unsigned int lawn_width = 50000;
    unsigned int lawn_length = 60000;

    initializeRuntimeConstants(lawn_width, lawn_length);

    EXPECT_EQ(MIN_MOVER_WIDTH, 50u);
    EXPECT_EQ(MAX_MOVER_WIDTH, 200u);
    EXPECT_EQ(MIN_MOVER_LENGTH, 50u);
    EXPECT_EQ(MAX_MOVER_LENGTH, 200u);
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
