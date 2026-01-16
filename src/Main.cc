/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    Creates the lawn, starts the simulation thread and opens the window to display everything.
    When you close the window, it cleanly stops the simulation and exits.
    TODO: description
*/

#include <QApplication>
#include <iostream>
#include <QTimer>
#include "Lawn.h"
#include "Mower.h"
#include "Config.h"
#include "Logger.h"
#include "FileLogger.h"
#include "StateSimulation.h"
#include "Engine.h"
#include "Visualizer.h"

using namespace std;

int main(int argc, char *argv[]) {
    constexpr unsigned int LAWN_WIDTH_CM = 500;
    constexpr unsigned int LAWN_LENGTH_CM = 1000;
    constexpr double SIMULATION_SPEED_MULTIPLIER = 1.0;
    
    constexpr unsigned int MOWER_WIDTH_CM = 50;
    constexpr unsigned int MOWER_LENGTH_CM = 50;
    constexpr unsigned int BLADE_DIAMETER_CM = 25;
    constexpr unsigned int MOWER_SPEED_CM_S = 50;
    
    QApplication app(argc, argv);
    
    cout << "[Main] Initializing components..." << endl;
    
    Config::initializeRuntimeConstants(LAWN_WIDTH_CM, LAWN_LENGTH_CM);
    
    cout << "[Main] Creating lawn: " << LAWN_WIDTH_CM << "x" << LAWN_LENGTH_CM << " cm" << endl;
    Lawn lawn(LAWN_WIDTH_CM, LAWN_LENGTH_CM);

    cout << "[Main] Creating Mower" << endl;
    Mower mower(MOWER_WIDTH_CM, MOWER_LENGTH_CM, BLADE_DIAMETER_CM, MOWER_SPEED_CM_S); 
    
    cout << "[Main] Creating Loggers" << endl;
    Logger logger; 
    FileLogger fileLogger("simulation.log");
    
    cout << "[Main] Creating StateSimulation" << endl;
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    
    // Test points
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 1/10, LAWN_LENGTH_CM* 1/10);
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 1/10, LAWN_LENGTH_CM * 9/10);
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 9/10, LAWN_LENGTH_CM* 1/10);
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 5/10, LAWN_LENGTH_CM* 5/10);
    simulation.simulateAddPoint(10, 10);
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 1/10, LAWN_LENGTH_CM* 9/10);
    simulation.simulateAddPoint(LAWN_WIDTH_CM* 9/10, LAWN_LENGTH_CM* 9/10);
    
    cout << "[Main] Initializing Engine" << endl;
    Engine engine(simulation); 
    engine.setSimulationSpeed(SIMULATION_SPEED_MULTIPLIER);

    cout << "[Main] Creating window" << endl;
    Visualizer visualizer(engine.getStateInterpolator()); 
    visualizer.setWindowTitle("Lawn Mower Simulator");    

    QTimer renderTimer;
    QObject::connect(&renderTimer, &QTimer::timeout, &visualizer, QOverload<>::of(&Visualizer::update));
    renderTimer.start(16);

    visualizer.show();
    
    engine.start();
    
    cout << "[Main] Starting event loop" << endl;
    const int result = app.exec();
    
    cout << "[Main] Stopping simulation" << endl;
    engine.stop();
    
    return result;
}
