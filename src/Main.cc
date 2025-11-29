#include <QApplication>
#include <QThread>
#include <iostream>
#include "Lawn.h"
#include "LawnVisualizer.h"

int main(int argc, char *argv[]) {
    std::cout << "=== Lawn Mower Simulator - Visualization Test ===" << std::endl;
    
    QApplication app(argc, argv);
    
    Lawn lawn(500, 400);
    
    
    auto fields = lawn.getFields();
    LawnVisualizer visualizer(&lawn);
    visualizer.setWindowTitle("Lawn Mower Simulator");
    
    visualizer.show();
    app.processEvents();
    
    unsigned int total_rows = fields.size();
    unsigned int total_cols = fields[0].size();
    unsigned int half_rows = total_rows / 2;
    
    unsigned int mowed_count = 0;
    unsigned int refresh_interval = 10;
    
    for (unsigned int row = 0; row < half_rows; row++) {
        for (unsigned int col = 0; col < total_cols; col++) {
            lawn.cutGrassOnField({col, row});
            mowed_count++;
        }
        
        if (row % refresh_interval == 0) {
            visualizer.refresh();
            app.processEvents();
            QThread::msleep(5);
        }
    }
    
    visualizer.refresh();
    app.processEvents();    
    return app.exec();
}
