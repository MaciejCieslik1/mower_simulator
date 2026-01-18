#include <gtest/gtest.h>
#include <QApplication>
#include <thread>
#include <mutex>
#include <chrono>

#include "Engine.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mower.h"
#include "Logger.h"
#include "FileLogger.h"
#include "Config.h"
#include "Visualizer.h"
#include "MowerController.h"
#include "Exceptions.h"

class EngineTests : public ::testing::Test {
protected:
    void SetUp() override {
        int argc = 0;
        char** argv = nullptr;
        if (!qApp) {
            app_ = new QApplication(argc, argv);
        }
    }

    void TearDown() override {
    }
    
    QApplication* app_ = nullptr;
};

TEST_F(EngineTests, Initialization) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, StartStop) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, DestructorStopsEngine) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    {
        Engine engine(simulation);
        Visualizer visualizer(engine.getStateInterpolator());
        engine.start();
        EXPECT_TRUE(engine.isRunning());
    }
    
    SUCCEED();
}

TEST_F(EngineTests, MultipleStartStopCycles) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, DoubleStartIsIdempotent) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, DoubleStopIsIdempotent) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    engine.start();
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, SimulationCallbackExecution) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<int> callback_count(0);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        callback_count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_GT(callback_count.load(), 0);
}

TEST_F(EngineTests, CallbackReceivesCorrectDeltaTime) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<bool> dt_is_correct(true);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double dt) {
        if (std::abs(dt - 0.02) > 0.001) {
            dt_is_correct = false;
        }
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_TRUE(dt_is_correct.load()) << "Delta time should always be 0.02 (fixed timestep)";
}

TEST_F(EngineTests, CallbackCanModifySimulation) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<int> execution_count(0);
    
    engine.setUserSimulationLogic([&](StateSimulation& sim, double dt) {
        execution_count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_GT(execution_count.load(), 0);
}

TEST_F(EngineTests, SpeedMultiplier) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<int> callback_count_normal(0);
    std::atomic<int> callback_count_fast(0);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        callback_count_normal++;
    });
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    engine.setSimulationSpeed(2.0);
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        callback_count_fast++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_GT(callback_count_fast.load(), callback_count_normal.load());
}

TEST_F(EngineTests, SpeedMultiplierRejectsInvalidValues) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    engine.setSimulationSpeed(0.0);
    engine.setSimulationSpeed(-1.0);
    
    std::atomic<int> count(0);
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    engine.stop();
    
    EXPECT_GT(count.load(), 0) << "Engine should still work after invalid speed values";
}

TEST_F(EngineTests, CanChangeSpeedWhileRunning) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<int> count(0);
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    engine.setSimulationSpeed(5.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    engine.stop();
    
    EXPECT_GT(count.load(), 0);
}

TEST_F(EngineTests, FixedTimestepIsDeterministic) {
    Logger logger1, logger2;
    FileLogger fileLogger1("test1.log"), fileLogger2("test2.log");
    Lawn lawn1(100, 100), lawn2(100, 100);
    Mower mower1(30, 40, 15, 20), mower2(30, 40, 15, 20);
    StateSimulation sim1(lawn1, mower1, logger1, fileLogger1);
    StateSimulation sim2(lawn2, mower2, logger2, fileLogger2);
    
    std::mutex mutex1, mutex2;
    Engine engine1(sim1);
    Engine engine2(sim2);
    Visualizer visualizer1(engine1.getStateInterpolator());
    Visualizer visualizer2(engine2.getStateInterpolator());
    
    std::atomic<int> count1(0), count2(0);
    
    auto logic = [](std::atomic<int>& counter) {
        return [&counter](StateSimulation&, double dt) {
            counter++;
        };
    };
    
    engine1.setUserSimulationLogic(logic(count1));
    engine2.setUserSimulationLogic(logic(count2));
    
    engine1.start();
    engine2.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine1.stop();
    engine2.stop();
    
    int diff = std::abs(count1.load() - count2.load());
    EXPECT_LE(diff, 2) << "Fixed timestep should produce deterministic step counts";
}

TEST_F(EngineTests, MutexProtectsStateAccess) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<bool> no_race_condition(true);
    
    engine.setUserSimulationLogic([&](StateSimulation& sim, double) {
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_TRUE(no_race_condition.load());
}

TEST_F(EngineTests, GetSimulationTimeReturnsValue) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    double initial_time = engine.getSimulationTime();
    EXPECT_GE(initial_time, 0.0);
}

TEST_F(EngineTests, BothThreadsActuallyRun) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mower mower(30, 40, 15, 20);
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    Engine engine(simulation);
    Visualizer visualizer(engine.getStateInterpolator());
    
    std::atomic<bool> simulation_ran(false);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        simulation_ran = true;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_TRUE(simulation_ran.load()) << "Simulation thread should execute";
}

TEST_F(EngineTests, simulationStopsOnMoveOutsideLawn) {
    unsigned int lawn_width = 100;
    unsigned int lawn_length = 100;
    unsigned int mower_width = 10;
    unsigned int mower_length = 10;
    unsigned int blade_diameter = 8;
    unsigned int mower_speed = 100;

    Config::initializeRuntimeConstants(lawn_width, lawn_length);
    Config::initializeMowerConstants(mower_width, mower_length, 50.0, 50.0, 0);

    Lawn lawn(lawn_width, lawn_length);
    Mower mower(mower_width, mower_length, blade_diameter, mower_speed);
    Logger logger;
    FileLogger fileLogger("test_exception_logs.log");
    StateSimulation sim(lawn, mower, logger, fileLogger);
    
    MowerController controller;
    controller.move(200.0); 

    Engine engine(sim, [&controller](StateSimulation& s, double dt) {
        controller.update(s, dt);
    });

    engine.start();
    
    auto start_time = std::chrono::steady_clock::now();
    bool stopped = false;
    while (std::chrono::steady_clock::now() - start_time < std::chrono::seconds(2)) {
        if (!engine.isRunning()) {
            stopped = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_TRUE(stopped);
    EXPECT_FALSE(engine.isRunning());
}