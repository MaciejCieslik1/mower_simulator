/* 
    Author: Hanna Biegacz
    
    LawnSimulationView implementation. 
*/
#include "simulation/LawnSimulationView.h"
#include "../../include/StateSimulation.h"
#include "../../include/Lawn.h" // Needed to access Lawn methods via StateSimulation
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <mutex>
#include <QMetaObject>

using namespace std;

const QColor LawnSimulationView::UNMOWED_GRASS_COLOR = QColor(119, 221, 118);  
const QColor LawnSimulationView::MOWED_GRASS_COLOR = QColor(152, 118, 85);     
const QColor LawnSimulationView::GRID_LINE_COLOR = QColor(200, 200, 200, 100);

LawnSimulationView::LawnSimulationView(StateSimulation& simulation, mutex& simulation_mutex, QWidget* parent)
    : QWidget(parent), simulation_(simulation), simulation_mutex_(simulation_mutex) {
    
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

LawnSimulationView::~LawnSimulationView() {
}


void LawnSimulationView::triggerRepaint() {
    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

QSize LawnSimulationView::sizeHint() const {
    return QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

QSize LawnSimulationView::minimumSizeHint() const {
    return QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

void LawnSimulationView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    drawLawnGrid(painter);
}

void LawnSimulationView::drawLawnGrid(QPainter& painter) {
    lock_guard<mutex> lock(simulation_mutex_);
    
    const Lawn& lawn = simulation_.getLawn();
    const auto fields = lawn.getFields();
    
    if (fields.empty()) return;

    const unsigned int num_rows = fields.size();
    const unsigned int num_cols = fields[0].size();
    
    const double field_width_px = static_cast<double>(width()) / num_cols;
    const double field_height_px = static_cast<double>(height()) / num_rows;
    const bool draw_grid_lines = (field_width_px >= 2.0 && field_height_px >= 2.0);
    
    for (unsigned int row = 0; row < num_rows; ++row) {
        for (unsigned int col = 0; col < num_cols; ++col) {
            const bool is_mowed = fields[row][col];
            
            const double x = col * field_width_px;
            const double y = height() - (row + 1) * field_height_px;
            
            const QColor fill_color = is_mowed ? MOWED_GRASS_COLOR : UNMOWED_GRASS_COLOR;
            
            painter.fillRect(QRectF(x, y, field_width_px, field_height_px), fill_color);
            
            if (draw_grid_lines) {
                painter.setPen(GRID_LINE_COLOR);
                painter.drawRect(QRectF(x, y, field_width_px, field_height_px));
            }
        }
    }
}

double LawnSimulationView::calculateFieldPixelWidth() const {
    lock_guard<mutex> lock(simulation_mutex_);
    const auto fields = simulation_.getLawn().getFields();
    if (fields.empty() || fields[0].empty()) {
        return 0.0;
    }
    
    const unsigned int num_cols = fields[0].size();
    return static_cast<double>(width()) / num_cols;
}

double LawnSimulationView::calculateFieldPixelHeight() const {
    lock_guard<mutex> lock(simulation_mutex_);
    const auto fields = simulation_.getLawn().getFields();
    if (fields.empty()) {
        return 0.0;
    }
    
    const unsigned int num_rows = fields.size();
    return static_cast<double>(height()) / num_rows;
}
