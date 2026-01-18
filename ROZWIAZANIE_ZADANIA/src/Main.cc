/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    This is the main entry point of the application. 
    It initializes the simulation components (Lawn, Mower, Engine, Visualizer) and launches the GUI.
    
    Configuration: Allows the user to define simulation constants (lawn size, mower speed, etc.).
    Custom Logic: The 'customUserLogic' function is where the user programs the mower's path.
*/

#include <QApplication>
#include <iostream>
#include <QTimer>
#include <cmath>
#include "Lawn.h"
#include "Mower.h"
#include "Config.h"
#include "Logger.h"
#include "FileLogger.h"
#include "StateSimulation.h"
#include "Engine.h"
#include "Visualizer.h"
#include "MowerController.h"

using namespace std;

// HERE THE USER CAN DEFINE THE SIMULATION PARAMETERS
    constexpr unsigned int LAWN_WIDTH_CM = 800;
    constexpr unsigned int LAWN_LENGTH_CM = 600;
    constexpr double       SIMULATION_SPEED_MULTIPLIER = 1.0;
    constexpr unsigned int MOWER_WIDTH_CM = 50;
    constexpr unsigned int MOWER_LENGTH_CM = 50;
    constexpr unsigned int BLADE_DIAMETER_CM = 50;
    constexpr unsigned int MOWER_SPEED_CM_S = 100;
    constexpr const char*  LOG_PATH = "../simulation_logs.log";
    constexpr int          TARGET_FPS = 100;
    constexpr int          RENDER_INTERVAL_MS = 1000 / TARGET_FPS;


void customUserLogic(MowerController& controller) {
    
    static double distance_p1_p0 = 0.0;
    static double distance_p1_p2 = 0.0;  
    controller.addPoint(LAWN_WIDTH_CM * 0.5, LAWN_LENGTH_CM * 0.29); // Point 0
    controller.addPoint(LAWN_WIDTH_CM * 0.5, LAWN_LENGTH_CM * 0.55); // Point 1 --- centre of the 8
    controller.addPoint(LAWN_WIDTH_CM * 0.5, LAWN_LENGTH_CM * 0.75); // Point 2

    controller.setMowing(false);
    controller.moveToPoint(1);

    controller.getDistanceToPoint(0, distance_p1_p0);
    controller.getDistanceToPoint(2, distance_p1_p2);



    // DRAWING FIRST CIRCLE
    controller.rotateTowardsPoint(0);
    controller.rotate(90);
    controller.setMowing(true);

    for (double angle = 0; angle < 360; angle += 1) {
        controller.move(&distance_p1_p0, (M_PI / 180.0));
        controller.rotate(-1);
    }

    // DRAWING SECOND CIRCLE
    controller.setMowing(false);
    controller.moveToPoint(1);
    controller.rotateTowardsPoint(2);
    controller.rotate(-90);
    controller.setMowing(true);
    for (double angle = 0; angle < 360; angle += 1) {
        controller.rotate(1);
        controller.move(&distance_p1_p2, (M_PI / 180.0));
    }

    controller.deletePoint(0);
    controller.deletePoint(1);
    controller.deletePoint(2);
}


// USERS SHOULD NOT HAVE TO CHANGE BELOW THIS LINE

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    cout << "[Main] Initializing components..." << endl;
    
    cout << "[Main] Creating lawn: " << LAWN_WIDTH_CM << "x" << LAWN_LENGTH_CM << " cm" << endl;
    Lawn lawn(LAWN_WIDTH_CM, LAWN_LENGTH_CM);

    cout << "[Main] Creating Mower" << endl;
    Mower mower(MOWER_WIDTH_CM, MOWER_LENGTH_CM, BLADE_DIAMETER_CM, MOWER_SPEED_CM_S); 
    
    cout << "[Main] Creating Loggers" << endl;
    Logger logger; 
    FileLogger fileLogger(LOG_PATH);
    
    cout << "[Main] Creating StateSimulation" << endl;
    StateSimulation simulation(lawn, mower, logger, fileLogger);
    
    cout << "[Main] Setting up MowerController and user logic" << endl;
    MowerController controller;
    customUserLogic(controller);

    cout << "[Main] Initializing Engine" << endl;
    Engine engine(simulation, 
        [&controller](StateSimulation& sim, double dt) {
            controller.update(sim, dt);
        }, 
        [&app](const string& error) {
            QMetaObject::invokeMethod(&app, "quit", Qt::QueuedConnection);
        }
    ); 
    engine.setSimulationSpeed(SIMULATION_SPEED_MULTIPLIER);
    
    cout << "[Main] Creating window" << endl;
    Visualizer visualizer(engine.getStateInterpolator()); 
    visualizer.setWindowTitle("Lawn Mower Simulator");    

    QTimer renderTimer;
    QObject::connect(&renderTimer, &QTimer::timeout, &visualizer, QOverload<>::of(&Visualizer::update));
    renderTimer.start(RENDER_INTERVAL_MS);
    visualizer.show();
    engine.start();
    
    cout << "[Main] Starting event loop" << endl;
    const int result = app.exec();
    
    cout << "[Main] Stopping simulation" << endl;
    engine.stop();
    return result;
}
