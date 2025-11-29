#include <gtest/gtest.h>
#include <QApplication>
#include "simulation/LawnSimulationView.h"
#include "Lawn.h"
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

class LawnSimulationViewTests : public ::testing::Test {
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
        lawn_mutex_ = new mutex();
        view_ = new LawnSimulationView(lawn_, *lawn_mutex_);
    }
    
    void TearDown() override {
        delete view_;
        delete lawn_mutex_;
        delete lawn_;
    }
    
    static QApplication* app_;
    Lawn* lawn_;
    mutex* lawn_mutex_;
    LawnSimulationView* view_;
};

QApplication* LawnSimulationViewTests::app_ = nullptr;

TEST_F(LawnSimulationViewTests, ConstructorCreatesValidWidget) {
    EXPECT_NE(nullptr, view_);
}

TEST_F(LawnSimulationViewTests, ConstructorThrowsOnNullLawn) {
    EXPECT_THROW({
        LawnSimulationView invalid_view(nullptr, *lawn_mutex_);
    }, invalid_argument);
}

TEST_F(LawnSimulationViewTests, DefaultWindowSize) {
    constexpr int EXPECTED_WIDTH = 800;
    constexpr int EXPECTED_HEIGHT = 600;
    
    QSize hint = view_->sizeHint();
    
    EXPECT_EQ(EXPECTED_WIDTH, hint.width());
    EXPECT_EQ(EXPECTED_HEIGHT, hint.height());
}

TEST_F(LawnSimulationViewTests, MinimumWindowSize) {
    constexpr int EXPECTED_MIN_WIDTH = 400;
    constexpr int EXPECTED_MIN_HEIGHT = 300;
    
    QSize min_hint = view_->minimumSizeHint();
    
    EXPECT_EQ(EXPECTED_MIN_WIDTH, min_hint.width());
    EXPECT_EQ(EXPECTED_MIN_HEIGHT, min_hint.height());
}

TEST_F(LawnSimulationViewTests, StartSimulationWithValidFPS) {
    constexpr int TEST_FPS = 30;
    
    EXPECT_NO_THROW(view_->startSimulation(TEST_FPS));
    
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, StartSimulationWithDefaultFPS) {
    EXPECT_NO_THROW(view_->startSimulation());
    
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, StopSimulationWithoutStart) {
    EXPECT_NO_THROW(view_->stopSimulation());
}

TEST_F(LawnSimulationViewTests, StartSimulationTwice) {
    view_->startSimulation(30);
    
    EXPECT_NO_THROW(view_->startSimulation(60));
    
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, StartStopMultipleTimes) {
    view_->startSimulation(30);
    view_->stopSimulation();
    
    view_->startSimulation(60);
    view_->stopSimulation();
    
    EXPECT_NO_THROW(view_->startSimulation(45));
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, RefreshDoesNotCrash) {
    EXPECT_NO_THROW(view_->refresh());
}

TEST_F(LawnSimulationViewTests, RefreshWithRunningSimulation) {
    view_->startSimulation(30);
    
    EXPECT_NO_THROW(view_->refresh());
    
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, InvalidFPSIsHandledGracefully) {
    EXPECT_NO_THROW(view_->startSimulation(0));
    EXPECT_NO_THROW(view_->startSimulation(-10));
    
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, WidgetCanBeShownAndHidden) {
    EXPECT_NO_THROW(view_->show());
    EXPECT_TRUE(view_->isVisible());
    
    EXPECT_NO_THROW(view_->hide());
    EXPECT_FALSE(view_->isVisible());
}

TEST_F(LawnSimulationViewTests, ThreadSafeAccessToLawn) {
    view_->startSimulation(30);
    
    thread modifier([this]() {
        lock_guard<mutex> lock(*lawn_mutex_);
        lawn_->cutGrassOnField({5, 5});
    });
    
    this_thread::sleep_for(chrono::milliseconds(50));
    
    EXPECT_NO_THROW(view_->refresh());
    
    modifier.join();
    view_->stopSimulation();
}

TEST_F(LawnSimulationViewTests, DestructorCleansUpProperly) {
    LawnSimulationView* temp_view = new LawnSimulationView(lawn_, *lawn_mutex_);
    temp_view->startSimulation(30);
    
    EXPECT_NO_THROW(delete temp_view);
}
