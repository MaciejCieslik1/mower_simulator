/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    Creates the lawn, starts the simulation thread and opens the window to display everything.
    When you close the window, it cleanly stops the simulation and exits.
*/

#include <QApplication>
#include <iostream>
#include "../include/Lawn.h"
#include "simulation/SimulationEngine.h"
#include "simulation/LawnSimulationView.h"

using namespace std;

int main(int argc, char *argv[]) {
    constexpr unsigned int LAWN_WIDTH_CM = 500;
    constexpr unsigned int LAWN_LENGTH_CM = 400;
    constexpr double SIMULATION_SPEED_MULTIPLIER = 10000.0;
    constexpr int DISPLAY_FPS = 30;
    
    QApplication app(argc, argv);
    
    cout << "[Main] Creating lawn: " << LAWN_WIDTH_CM << "x" << LAWN_LENGTH_CM << " cm" << endl;
    Lawn lawn(LAWN_WIDTH_CM, LAWN_LENGTH_CM);
    
    cout << "[Main] Initializing simulation engine" << endl;
    SimulationEngine engine(&lawn);
    engine.setSimulationSpeed(SIMULATION_SPEED_MULTIPLIER);
    
    cout << "[Main] Creating window" << endl;
    LawnSimulationView simulation_view(&lawn, engine.getLawnMutex()); 
    simulation_view.setWindowTitle("Lawn Mower Simulator");    
    
    simulation_view.show();
    simulation_view.startSimulation(DISPLAY_FPS);    
    engine.start();
    
    cout << "[Main] Starting event loop" << endl;
    const int result = app.exec();
    
    cout << "[Main] Stopping simulation" << endl;
    engine.stop();
    
    return result;
}
