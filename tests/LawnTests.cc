/* 
    Author: Maciej Cieslik
    
    Tests Lawn class methods.
*/

#include <gtest/gtest.h>
#include "../include/Lawn.h"

TEST(InitializeRuntimeConstantsTest, Getters) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    Lawn lawn = Lawn(lawn_width, lawn_length);

    unsigned int width = lawn.getWidth();
    unsigned int length = lawn.getWidth();

    EXPECT_EQ(width, lawn_width);
    EXPECT_EQ(length, lawn_length);
}
