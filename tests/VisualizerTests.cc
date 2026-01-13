#include <gtest/gtest.h>
#include <QApplication>
#include "Visualizer.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mover.h"
#include "Logger.h"
#include "FileLogger.h"
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

class VisualizerTests : public ::testing::Test {
protected:
    static void SetUpTestSuite() {
        int argc = 0;
        char** argv = nullptr;
        app_ = new QApplication(argc, argv);
    }
    
    static void TearDownTestSuite() {
        delete app_;
        app_ = nullptr;
    }
    
    void SetUp() override {
        lawn_ = new Lawn(100, 80);
        mover_ = new Mover(30, 40, 15, 20);
        logger_ = new Logger();
        fileLogger_ = new FileLogger("test_visualizer.log");
        
        simulation_ = new StateSimulation(*lawn_, *mover_, *logger_, *fileLogger_);
        simulation_mutex_ = new mutex();
        
        visualizer_ = new Visualizer(*simulation_, *simulation_mutex_);
    }
    
    void TearDown() override {
        delete visualizer_;
        delete simulation_mutex_;
        delete simulation_;
        delete fileLogger_;
        delete logger_;
        delete mover_;
        delete lawn_;
    }
    
    static QApplication* app_;
    Lawn* lawn_;
    Mover* mover_;
    Logger* logger_;
    FileLogger* fileLogger_;
    StateSimulation* simulation_;
    mutex* simulation_mutex_;
    Visualizer* visualizer_;
};

QApplication* VisualizerTests::app_ = nullptr;

TEST_F(VisualizerTests, ConstructorCreatesValidWidget) {
    EXPECT_NE(nullptr, visualizer_);
}

TEST_F(VisualizerTests, DefaultWindowSize) {
    constexpr int EXPECTED_WIDTH = 800;
    constexpr int EXPECTED_HEIGHT = 600;
    
    QSize hint = visualizer_->sizeHint();
    
    EXPECT_EQ(EXPECTED_WIDTH, hint.width());
    EXPECT_EQ(EXPECTED_HEIGHT, hint.height());
}

TEST_F(VisualizerTests, MinimumWindowSize) {
    constexpr int EXPECTED_MIN_WIDTH = 400;
    constexpr int EXPECTED_MIN_HEIGHT = 300;
    
    QSize min_hint = visualizer_->minimumSizeHint();
    
    EXPECT_EQ(EXPECTED_MIN_WIDTH, min_hint.width());
    EXPECT_EQ(EXPECTED_MIN_HEIGHT, min_hint.height());
}

TEST_F(VisualizerTests, TriggerRepaintDoesNotCrash) {
    EXPECT_NO_THROW(visualizer_->triggerRepaint());
}

TEST_F(VisualizerTests, WidgetCanBeShownAndHidden) {
    EXPECT_NO_THROW(visualizer_->show());
    EXPECT_TRUE(visualizer_->isVisible());
    
    EXPECT_NO_THROW(visualizer_->hide());
    EXPECT_FALSE(visualizer_->isVisible());
}
