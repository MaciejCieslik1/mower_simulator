/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    Creates the lawn, starts the simulation thread and opens the window to display everything.
    When you close the window, it cleanly stops the simulation and exits.
*/

#include <QApplication>
#include <iostream>
#include "../include/Lawn.h"
#include "../include/Mover.h"
#include "../include/Config.h"
#include "../include/Logger.h"
#include "../include/FileLogger.h"
#include "../include/StateSimulation.h"
#include "Engine.h"
#include "Visualizer.h"

using namespace std;

int main(int argc, char *argv[]) {
    constexpr unsigned int LAWN_WIDTH_CM = 500;
    constexpr unsigned int LAWN_LENGTH_CM = 400;
    constexpr double SIMULATION_SPEED_MULTIPLIER = 10000.0;
    
    constexpr unsigned int MOVER_WIDTH_CM = 30;
    constexpr unsigned int MOVER_LENGTH_CM = 40;
    constexpr unsigned int BLADE_DIAMETER_CM = 15;
    constexpr unsigned int MOVER_SPEED_CM_S = 20;
    
    QApplication app(argc, argv);
    
    cout << "[Main] Initializing components..." << endl;
    
    Config::initializeRuntimeConstants(LAWN_WIDTH_CM, LAWN_LENGTH_CM);
    
    cout << "[Main] Creating lawn: " << LAWN_WIDTH_CM << "x" << LAWN_LENGTH_CM << " cm" << endl;
    Lawn lawn(LAWN_WIDTH_CM, LAWN_LENGTH_CM);

    cout << "[Main] Creating Mover" << endl;
    Mover mover(MOVER_WIDTH_CM, MOVER_LENGTH_CM, BLADE_DIAMETER_CM, MOVER_SPEED_CM_S); 
    
    cout << "[Main] Creating Loggers" << endl;
    Logger logger; 
    FileLogger fileLogger("simulation.log");
    
    cout << "[Main] Creating StateSimulation" << endl;
    StateSimulation simulation(lawn, mover, logger, fileLogger);
    
    std::mutex simulation_mutex;
    
    cout << "[Main] Creating window" << endl;
    Visualizer visualizer(simulation, simulation_mutex); 
    visualizer.setWindowTitle("Lawn Mower Simulator");    
    
    cout << "[Main] Initializing Engine" << endl;
    Engine engine(simulation, visualizer); 
    engine.setSimulationSpeed(SIMULATION_SPEED_MULTIPLIER);

    visualizer.show();
    
    engine.start();
    
    cout << "[Main] Starting event loop" << endl;
    const int result = app.exec();
    
    cout << "[Main] Stopping simulation" << endl;
    engine.stop();
    
    return result;
}
