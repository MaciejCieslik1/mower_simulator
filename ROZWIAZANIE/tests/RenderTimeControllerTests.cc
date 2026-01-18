#include <gtest/gtest.h>
#include "../include/RenderTimeController.h"
#include "../include/StateInterpolator.h"

using namespace std;

TEST(RenderTimeControllerTest, initialSmoothedTimeIsZero) {
    StateInterpolator interpolator;
    RenderTimeController controller(interpolator);

    double smoothed_time = controller.getSmoothedTime();

    EXPECT_DOUBLE_EQ(smoothed_time, 0.0);
}

TEST(RenderTimeControllerTest, updateAdvancesTime) {
    StateInterpolator interpolator;
    RenderTimeController controller(interpolator);

    controller.update(100.0);
    double smoothed_time = controller.getSmoothedTime();

    SimulationSnapshot snapshot;
    snapshot.simulation_time_ = 1000.0;
    interpolator.addSimulationSnapshot(snapshot);
    
    controller.update(100.0);
    smoothed_time = controller.getSmoothedTime();
    
    EXPECT_GT(smoothed_time, 0.0);
}


TEST(RenderTimeControllerTest, updateFollowsSpeedMultiplier) {
    StateInterpolator interpolator;
    SimulationSnapshot snapshot;
    snapshot.simulation_time_ = 5000.0;
    interpolator.addSimulationSnapshot(snapshot);

    // SLOW
    RenderTimeController controller_slow(interpolator);
    interpolator.setSimulationSpeedMultiplier(1.0);
    controller_slow.update(0.0); 
    double initial_slow = controller_slow.getSmoothedTime();
    controller_slow.update(100.0); 
    double delta_slow = controller_slow.getSmoothedTime() - initial_slow;
    
    // FAST
    RenderTimeController controller_fast(interpolator);
    interpolator.setSimulationSpeedMultiplier(2.0);
    controller_fast.update(0.0); 
    double initial_fast = controller_fast.getSmoothedTime();
    controller_fast.update(100.0); 
    double delta_fast = controller_fast.getSmoothedTime() - initial_fast;

    EXPECT_GT(delta_fast, delta_slow);
}
