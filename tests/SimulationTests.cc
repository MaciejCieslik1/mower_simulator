#include <gtest/gtest.h>
#include "simulation/SimulationEngine.h"
#include "Lawn.h"
#include <thread>
#include <chrono>

using namespace std;

class SimulationEngineTests : public ::testing::Test {
protected:
    void SetUp() override {
        lawn_ = new Lawn(100, 100);
        engine_ = new SimulationEngine(lawn_);
    }
    
    void TearDown() override {
        delete engine_;
        delete lawn_;
    }
    
    Lawn* lawn_;
    SimulationEngine* engine_;
};

TEST_F(SimulationEngineTests, InitialStateIsNotRunning) {
    EXPECT_FALSE(engine_->isRunning());
}

TEST_F(SimulationEngineTests, DefaultSpeedIsOne) {
    EXPECT_DOUBLE_EQ(1.0, engine_->getSimulationSpeed());
}

TEST_F(SimulationEngineTests, SetAndGetSimulationSpeed) {
    constexpr double TEST_SPEED = 5.5;
    
    engine_->setSimulationSpeed(TEST_SPEED);
    
    EXPECT_DOUBLE_EQ(TEST_SPEED, engine_->getSimulationSpeed());
}

TEST_F(SimulationEngineTests, SetSpeedToZeroShouldBeIgnored) {
    constexpr double INITIAL_SPEED = 2.0;
    engine_->setSimulationSpeed(INITIAL_SPEED);
    
    engine_->setSimulationSpeed(0.0);
    
    EXPECT_DOUBLE_EQ(INITIAL_SPEED, engine_->getSimulationSpeed());
}

TEST_F(SimulationEngineTests, SetNegativeSpeedShouldBeIgnored) {
    constexpr double INITIAL_SPEED = 3.0;
    engine_->setSimulationSpeed(INITIAL_SPEED);
    
    engine_->setSimulationSpeed(-1.5);
    
    EXPECT_DOUBLE_EQ(INITIAL_SPEED, engine_->getSimulationSpeed());
}

TEST_F(SimulationEngineTests, StartChangesRunningStateToTrue) {
    engine_->start();
    
    EXPECT_TRUE(engine_->isRunning());
    
    engine_->stop();
}

TEST_F(SimulationEngineTests, StopChangesRunningStateToFalse) {
    engine_->start();
    ASSERT_TRUE(engine_->isRunning());
    
    engine_->stop();
    
    EXPECT_FALSE(engine_->isRunning());
}

TEST_F(SimulationEngineTests, StartTwiceShouldNotCrash) {
    engine_->start();
    engine_->start();
    
    EXPECT_TRUE(engine_->isRunning());
    
    engine_->stop();
}

TEST_F(SimulationEngineTests, StopWithoutStartShouldNotCrash) {
    EXPECT_NO_THROW(engine_->stop());
    EXPECT_FALSE(engine_->isRunning());
}

TEST_F(SimulationEngineTests, SimulationActuallyMowsGrass) {
    constexpr double FAST_SPEED = 1000.0;
    engine_->setSimulationSpeed(FAST_SPEED);
    
    auto fields_before = lawn_->getFields();
    unsigned int mowed_before = 0;
    for (const auto& row : fields_before) {
        for (bool field : row) {
            if (field) {
                ++mowed_before;
            }
        }
    }
    
    engine_->start();
    this_thread::sleep_for(chrono::milliseconds(100));
    engine_->stop();
    
    auto fields_after = lawn_->getFields();
    unsigned int mowed_after = 0;
    for (const auto& row : fields_after) {
        for (bool field : row) {
            if (field) {
                ++mowed_after;
            }
        }
    }
    
    EXPECT_GT(mowed_after, mowed_before);
}

TEST_F(SimulationEngineTests, GetLawnMutexReturnsValidReference) {
    mutex& lawn_mutex = engine_->getLawnMutex();
    
    EXPECT_NO_THROW({
        lock_guard<mutex> lock(lawn_mutex);
    });
}

TEST_F(SimulationEngineTests, ConstructorThrowsOnNullLawn) {
    EXPECT_THROW({
        SimulationEngine null_engine(nullptr);
    }, invalid_argument);
}
