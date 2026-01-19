#include <gtest/gtest.h>
#include <memory>
#include <cmath>
#include "StateSimulation.h"
#include "commands/AddPointCommand.h"
#include "commands/DeletePointCommand.h"
#include "commands/GetDistanceToPointCommand.h"
#include "commands/MoveCommand.h"
#include "commands/MoveToPointCommand.h"
#include "commands/MowingOptionCommand.h"
#include "commands/RotateCommand.h"
#include "commands/RotateTowardsPointCommand.h"
#include "commands/GetCurrentPositionCommand.h"
#include "commands/GetCurrentAngleCommand.h"
#include "MathHelper.h"
#include "Lawn.h"
#include "Mower.h"
#include "Logger.h"
#include "FileLogger.h"
#include "Config.h"

class CommandTests : public ::testing::Test {
protected:
    void SetUp() override {
        Config::initializeRuntimeConstants(1000, 1000); 
        Config::initializeMowerConstants(50, 50, 0, 0, 0);

        lawn = std::make_unique<Lawn>(1000, 1000);
        mower = std::make_unique<Mower>(50, 50, 20, 10); 
        logger = std::make_unique<Logger>();
        fileLogger = std::make_unique<FileLogger>("test_command_log.txt");
        
        simulation = std::make_unique<StateSimulation>(*lawn, *mower, *logger, *fileLogger);
    }
    
    void TearDown() override {
    }

    std::unique_ptr<Lawn> lawn;
    std::unique_ptr<Mower> mower;
    std::unique_ptr<Logger> logger;
    std::unique_ptr<FileLogger> fileLogger;
    std::unique_ptr<StateSimulation> simulation;
};

TEST_F(CommandTests, AddPointCommandAddsPointToSimulation) {
    AddPointCommand command(10.0, 20.0);
    command.execute(*simulation, 1.0);

    auto points = simulation->getPoints();
    ASSERT_FALSE(points.empty());
    EXPECT_DOUBLE_EQ(points.back().getX(), 10.0);
    EXPECT_DOUBLE_EQ(points.back().getY(), 20.0);
}

TEST_F(CommandTests, DeletePointCommandRemovesPointFromSimulation) {
    simulation->simulateAddPoint(10.0, 20.0);
    auto pointsBefore = simulation->getPoints();
    ASSERT_FALSE(pointsBefore.empty());
    unsigned int pointId = pointsBefore.back().getId();

    DeletePointCommand command(pointId);
    command.execute(*simulation, 1.0);

    auto pointsAfter = simulation->getPoints();
    EXPECT_EQ(pointsAfter.size(), pointsBefore.size() - 1);
}

TEST_F(CommandTests, MowingOptionCommandEnablesAndDisablesMowing) {
    MowingOptionCommand enableCommand(true);
    enableCommand.execute(*simulation, 1.0);
    EXPECT_TRUE(simulation->getMower().getIsMowing());

    MowingOptionCommand disableCommand(false);
    disableCommand.execute(*simulation, 1.0);
    EXPECT_FALSE(simulation->getMower().getIsMowing());
}

TEST_F(CommandTests, MoveCommandMovesMowerForward) {
    double initialX = simulation->getMower().getX();
    double initialY = simulation->getMower().getY(); 
    MoveCommand command(10.0); 
    
    while (!command.execute(*simulation, 0.1));
    
    double finalX = simulation->getMower().getX();
    double finalY = simulation->getMower().getY();
    
    // Mower moves based on angle. Check if position changed in general.
    EXPECT_TRUE(finalX != initialX || finalY != initialY); 
}

TEST_F(CommandTests, RotateCommandRotatesMower) {
    double initialAngle = simulation->getMower().getAngle();
    RotateCommand command(90); 
    
    while (!command.execute(*simulation, 0.1));

    double finalAngle = simulation->getMower().getAngle();
    unsigned short expectedAngle = (initialAngle + 90);
    if (expectedAngle >= 360) expectedAngle -= 360;
    
    EXPECT_EQ(finalAngle, expectedAngle);
}

TEST_F(CommandTests, GetDistanceToPointCommandCalculatesCorrectDistance) {
    simulation->simulateAddPoint(10.0, 0.0); 
    
    auto points = simulation->getPoints();
    ASSERT_FALSE(points.empty());
    unsigned int pointId = points.back().getId();
    
    double distance = 0.0;
    GetDistanceToPointCommand command(pointId, distance);
    command.execute(*simulation, 1.0);
    
    double mowerX = simulation->getMower().getX();
    double mowerY = simulation->getMower().getY();
    double expectedDist = std::sqrt(std::pow(10.0 - mowerX, 2) + std::pow(0.0 - mowerY, 2));
    
    EXPECT_NEAR(distance, expectedDist, 0.001);
}

TEST_F(CommandTests, RotateTowardsPointCommandRotatesToFacePoint) {
    simulation->simulateAddPoint(100.0, 100.0); 
    auto points = simulation->getPoints();
    unsigned int pointId = points.back().getId();

    RotateTowardsPointCommand command(pointId);
    int safety = 0;
    while (!command.execute(*simulation, 0.1) && safety++ < 1000);
    
    ASSERT_LT(safety, 1000);
}

TEST_F(CommandTests, MoveToPointCommandMovesMowerToPoint) {
    simulation->simulateAddPoint(50.0, 50.0); 
    auto points = simulation->getPoints();
    unsigned int pointId = points.back().getId();
    
    MoveToPointCommand command(pointId);
    
    int steps = 0;
    const int APP_TIMEOUT = 10000;
    while (!command.execute(*simulation, 0.1) && steps < APP_TIMEOUT) {
        steps++;
    }
    
    ASSERT_LT(steps, APP_TIMEOUT);
    
    double mowerX = simulation->getMower().getX();
    double mowerY = simulation->getMower().getY();
    
    EXPECT_NEAR(mowerX, 50.0, 2.0);
    EXPECT_NEAR(mowerY, 50.0, 2.0);
}

TEST_F(CommandTests, GetCurrentPositionCommandRetrievesMowerPosition) {
    Config::initializeMowerConstants(50, 50, 100.0, 200.0, 0);
    mower = std::make_unique<Mower>(50, 50, 20, 10);
    simulation = std::make_unique<StateSimulation>(*lawn, *mower, *logger, *fileLogger);

    double outX = 0.0, outY = 0.0;
    GetCurrentPositionCommand command(outX, outY);
    command.execute(*simulation, 1.0);

    EXPECT_DOUBLE_EQ(outX, 100.0);
    EXPECT_DOUBLE_EQ(outY, 200.0);
}

TEST_F(CommandTests, GetCurrentAngleCommandRetrievesMowerAngle) {
    const unsigned short initialAngle = 45;
    Config::initializeMowerConstants(50, 50, 0, 0, initialAngle);
    mower = std::make_unique<Mower>(50, 50, 20, 10);
    simulation = std::make_unique<StateSimulation>(*lawn, *mower, *logger, *fileLogger);

    unsigned short outAngle = 0;
    GetCurrentAngleCommand command(outAngle);
    command.execute(*simulation, 1.0);

    EXPECT_EQ(outAngle, initialAngle);
}
