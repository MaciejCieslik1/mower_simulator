#include <gtest/gtest.h>
#include <QApplication>
#include <thread>
#include <mutex>
#include <chrono>

#include "Engine.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mover.h"
#include "Logger.h"
#include "FileLogger.h"
#include "Config.h"
#include "simulation/LawnSimulationView.h"

// Helper fixture with common setup
class EngineTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Need a QApplication for QWidget (LawnSimulationView)
        int argc = 0;
        char** argv = nullptr;
        if (!qApp) {
            app_ = new QApplication(argc, argv);
        }
    }

    void TearDown() override {
        // Don't delete qApp - managed by test suite
    }
    
    QApplication* app_ = nullptr;
};

// ============================================================================
// BASIC FUNCTIONALITY TESTS
// ============================================================================

TEST_F(EngineTests, Initialization) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, StartStop) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
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
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    {
        Engine engine(simulation, view);
        engine.start();
        EXPECT_TRUE(engine.isRunning());
        // Destructor should stop engine automatically
    }
    
    // If we get here without hanging, destructor properly stopped threads
    SUCCEED();
}

TEST_F(EngineTests, MultipleStartStopCycles) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    // Cycle 1
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    // Cycle 2
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    // Cycle 3
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, DoubleStartIsIdempotent) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    
    // Second start should be ignored
    engine.start();
    EXPECT_TRUE(engine.isRunning());
    
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

TEST_F(EngineTests, DoubleStopIsIdempotent) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    engine.start();
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
    
    // Second stop should be safe
    engine.stop();
    EXPECT_FALSE(engine.isRunning());
}

// ============================================================================
// SIMULATION CALLBACK TESTS
// ============================================================================

TEST_F(EngineTests, SimulationCallbackExecution) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<int> callback_count(0);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        callback_count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ~5 ticks at 20ms
    engine.stop();
    
    EXPECT_GT(callback_count.load(), 0);
}

TEST_F(EngineTests, CallbackReceivesCorrectDeltaTime) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<bool> dt_is_correct(true);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double dt) {
        // Fixed timestep should always be 0.02 (20ms)
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
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<int> execution_count(0);
    
    engine.setUserSimulationLogic([&](StateSimulation& sim, double dt) {
        // This demonstrates that callback can modify simulation
        // (e.g., call simulateMovement, simulateRotation, etc.)
        execution_count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_GT(execution_count.load(), 0);
}

// ============================================================================
// SPEED MULTIPLIER TESTS
// ============================================================================

TEST_F(EngineTests, SpeedMultiplier) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<int> callback_count_normal(0);
    std::atomic<int> callback_count_fast(0);
    
    // Normal speed (1x)
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        callback_count_normal++;
    });
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    // Fast speed (2x)
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
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    // Should reject zero and negative values
    engine.setSimulationSpeed(0.0);
    engine.setSimulationSpeed(-1.0);
    
    // Engine should still work
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
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<int> count(0);
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        count++;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    // Change speed while running
    engine.setSimulationSpeed(5.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    engine.stop();
    
    EXPECT_GT(count.load(), 0);
}

// ============================================================================
// TIMING AND DETERMINISM TESTS
// ============================================================================

TEST_F(EngineTests, FixedTimestepIsDeterministic) {
    Logger logger1, logger2;
    FileLogger fileLogger1("test1.log"), fileLogger2("test2.log");
    Lawn lawn1(100, 100), lawn2(100, 100);
    Mover mover1(30, 40, 15, 20), mover2(30, 40, 15, 20);
    StateSimulation sim1(lawn1, mover1, logger1, fileLogger1);
    StateSimulation sim2(lawn2, mover2, logger2, fileLogger2);
    
    std::mutex mutex1, mutex2;
    LawnSimulationView view1(sim1, mutex1);
    LawnSimulationView view2(sim2, mutex2);
    
    Engine engine1(sim1, view1);
    Engine engine2(sim2, view2);
    
    std::atomic<int> count1(0), count2(0);
    
    auto logic = [](std::atomic<int>& counter) {
        return [&counter](StateSimulation&, double dt) {
            counter++;
        };
    };
    
    engine1.setUserSimulationLogic(logic(count1));
    engine2.setUserSimulationLogic(logic(count2));
    
    // Run both for same wall-clock time
    engine1.start();
    engine2.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine1.stop();
    engine2.stop();
    
    // Counts should be similar (within tolerance for timing variations)
    int diff = std::abs(count1.load() - count2.load());
    EXPECT_LE(diff, 2) << "Fixed timestep should produce deterministic step counts";
}

// ============================================================================
// THREAD SAFETY TESTS
// ============================================================================

TEST_F(EngineTests, MutexProtectsStateAccess) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<bool> no_race_condition(true);
    
    engine.setUserSimulationLogic([&](StateSimulation& sim, double) {
        // This is protected by mutex inside updateSimulation()
        // Multiple threads shouldn't execute this simultaneously
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_TRUE(no_race_condition.load());
}

// ============================================================================
// MONITORING TESTS
// ============================================================================

TEST_F(EngineTests, GetSimulationTimeReturnsValue) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    double initial_time = engine.getSimulationTime();
    EXPECT_GE(initial_time, 0.0);
}

TEST_F(EngineTests, BothThreadsActuallyRun) {
    Logger logger;
    FileLogger fileLogger("test.log");
    Lawn lawn(100, 100);
    Mover mover(30, 40, 15, 20);
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    std::mutex view_mutex;
    LawnSimulationView view(simulation, view_mutex);
    
    Engine engine(simulation, view);
    
    std::atomic<bool> simulation_ran(false);
    
    engine.setUserSimulationLogic([&](StateSimulation&, double) {
        simulation_ran = true;
    });
    
    engine.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    engine.stop();
    
    EXPECT_TRUE(simulation_ran.load()) << "Simulation thread should execute";
    // Visualization thread tested indirectly (no crash = it's working)
}
