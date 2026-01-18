#include <gtest/gtest.h>
#include "../include/StateInterpolator.h"
#include "../include/SimulationSnapshot.h"

using namespace std;

TEST(StateInterpolatorTest, initialSpeedMultiplierIsOne) {
    StateInterpolator interpolator;
    
    double speed_multiplier = interpolator.getSpeedMultiplier();

    EXPECT_DOUBLE_EQ(speed_multiplier, 1.0);
}

TEST(StateInterpolatorTest, setSpeedMultiplierUpdatesValue) {
    StateInterpolator interpolator;
    double expected_multiplier = 2.5;

    interpolator.setSimulationSpeedMultiplier(expected_multiplier);
    double actual_multiplier = interpolator.getSpeedMultiplier();

    EXPECT_DOUBLE_EQ(actual_multiplier, expected_multiplier);
}

TEST(StateInterpolatorTest, setStaticSimulationDataUpdatesData) {
    StateInterpolator interpolator;
    StaticSimulationData data;
    data.lawn_width_ = 100;
    data.lawn_length_ = 200;
    data.width_cm_ = 50.0;
    data.length_cm = 100.0;
    data.blade_diameter_cm = 25.0;

    interpolator.setStaticSimulationData(data);
    StaticSimulationData retrieved_data = interpolator.getStaticSimulationData();

    EXPECT_EQ(retrieved_data.lawn_width_, data.lawn_width_);
    EXPECT_EQ(retrieved_data.lawn_length_, data.lawn_length_);
    EXPECT_DOUBLE_EQ(retrieved_data.width_cm_, data.width_cm_);
    EXPECT_DOUBLE_EQ(retrieved_data.length_cm, data.length_cm);
    EXPECT_DOUBLE_EQ(retrieved_data.blade_diameter_cm, data.blade_diameter_cm);
}

TEST(StateInterpolatorTest, getInterpolatedStateReturnsLatestWhenBufferOnlyOneSnapshot) {
    StateInterpolator interpolator;
    SimulationSnapshot snapshot;
    snapshot.simulation_time_ = 1000.0;
    snapshot.x_ = 10.0;
    snapshot.y_ = 20.0;
    interpolator.addSimulationSnapshot(snapshot);

    SimulationSnapshot result = interpolator.getInterpolatedState(500.0);

    EXPECT_DOUBLE_EQ(result.x_, snapshot.x_);
    EXPECT_DOUBLE_EQ(result.y_, snapshot.y_);
    EXPECT_DOUBLE_EQ(result.simulation_time_, snapshot.simulation_time_);
}

TEST(StateInterpolatorTest, getInterpolatedStateInterpolatesLinearValues) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    snapshot1.x_ = 10.0;
    snapshot1.y_ = 20.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2000.0;
    snapshot2.x_ = 20.0;
    snapshot2.y_ = 40.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double render_time = 1500.0;
    SimulationSnapshot result = interpolator.getInterpolatedState(render_time);

    EXPECT_DOUBLE_EQ(result.x_, 15.0);
    EXPECT_DOUBLE_EQ(result.y_, 30.0);
}

TEST(StateInterpolatorTest, getInterpolatedStateInterpolatesAngle) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    snapshot1.angle_ = 10.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2000.0;
    snapshot2.angle_ = 30.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double render_time = 1500.0;
    SimulationSnapshot result = interpolator.getInterpolatedState(render_time);

    EXPECT_DOUBLE_EQ(result.angle_, 20.0);
}

TEST(StateInterpolatorTest, getInterpolatedStateInterpolatesAngleCrossingZero) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    snapshot1.angle_ = 350.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2000.0;
    snapshot2.angle_ = 10.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double render_time = 1500.0;
    SimulationSnapshot result = interpolator.getInterpolatedState(render_time);

    // The shortest path from 350 to 10 is 20 degrees difference. Halfway is 350 + 10 = 360.
    EXPECT_DOUBLE_EQ(result.angle_, 360.0); 
}

TEST(StateInterpolatorTest, getInterpolatedStateReturnsEarliestIfRenderTimeBeforeBuffer) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    snapshot1.x_ = 10.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2000.0;
    snapshot2.x_ = 20.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double render_time = 500.0;
    SimulationSnapshot result = interpolator.getInterpolatedState(render_time);

    EXPECT_DOUBLE_EQ(result.x_, snapshot1.x_);
}

TEST(StateInterpolatorTest, getInterpolatedStateReturnsLatestIfRenderTimeAfterBuffer) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    snapshot1.x_ = 10.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2000.0;
    snapshot2.x_ = 20.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double render_time = 2500.0;
    SimulationSnapshot result = interpolator.getInterpolatedState(render_time);

    EXPECT_DOUBLE_EQ(result.x_, snapshot2.x_);
}

TEST(StateInterpolatorTest, getSimulationTimeReturnsLatestSnapshotTime) {
    StateInterpolator interpolator;
    
    SimulationSnapshot snapshot1;
    snapshot1.simulation_time_ = 1000.0;
    interpolator.addSimulationSnapshot(snapshot1);

    SimulationSnapshot snapshot2;
    snapshot2.simulation_time_ = 2500.0;
    interpolator.addSimulationSnapshot(snapshot2);

    double sim_time = interpolator.getSimulationTime();

    EXPECT_DOUBLE_EQ(sim_time, 2500.0);
}

TEST(StateInterpolatorTest, getSimulationTimeReturnsZeroIfEmpty) {
    StateInterpolator interpolator;
    
    double sim_time = interpolator.getSimulationTime();

    EXPECT_DOUBLE_EQ(sim_time, 0.0);
}
