#include <QApplication>
#include <iostream>
#include "../include/Lawn.h"
#include "simulation/SimulationEngine.h"
#include "simulation/LawnSimulationView.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    cout << "=== Lawn Mower Simulator - Threaded ===" << endl;
    
    Lawn lawn(500, 400);
    const auto fields = lawn.getFields();
    
    cout << "[Main] Lawn: " << fields[0].size() << " x " << fields.size() << " fields" << endl;

    SimulationEngine engine(&lawn);
    engine.setSimulationSpeed(10000.0);

    LawnSimulationView simulationView(&lawn, engine.getLawnMutex());  // ← BEZ &
    simulationView.setWindowTitle("Lawn Mower Simulator - Threaded");    
    
    simulationView.show();
    simulationView.startSimulation(30);    
    engine.start();

    cout << "[Main] Starting event loop" << endl;

    const int result = app.exec();
    
    cout << "[Main] Stopping simulation" << endl;
    engine.stop();
    
    return result;
}
