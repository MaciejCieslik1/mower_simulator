/* 
    Author: Hanna Biegacz
    
    Visualizer implementation. 
*/
#include "Visualizer.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <mutex>
#include <QMetaObject>

using namespace std;

const QColor Visualizer::UNMOWED_GRASS_COLOR = QColor(119, 221, 118);  
const QColor Visualizer::MOWED_GRASS_COLOR = QColor(152, 118, 85);     
const QColor Visualizer::GRID_LINE_COLOR = QColor(200, 200, 200, 100);

Visualizer::Visualizer(StateSimulation& simulation, mutex& simulation_mutex, QWidget* parent)
    : QWidget(parent), simulation_(simulation), simulation_mutex_(simulation_mutex) {
    
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

Visualizer::~Visualizer() {
}

void Visualizer::triggerRepaint() {
    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

QSize Visualizer::sizeHint() const {
    return QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

QSize Visualizer::minimumSizeHint() const {
    return QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

void Visualizer::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    drawLawnGrid(painter);
}

void Visualizer::drawLawnGrid(QPainter& painter) {
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

double Visualizer::calculateFieldPixelWidth() const {
    lock_guard<mutex> lock(simulation_mutex_);
    const auto fields = simulation_.getLawn().getFields();
    if (fields.empty() || fields[0].empty()) {
        return 0.0;
    }
    
    const unsigned int num_cols = fields[0].size();
    return static_cast<double>(width()) / num_cols;
}

double Visualizer::calculateFieldPixelHeight() const {
    lock_guard<mutex> lock(simulation_mutex_);
    const auto fields = simulation_.getLawn().getFields();
    if (fields.empty()) {
        return 0.0;
    }
    
    const unsigned int num_rows = fields.size();
    return static_cast<double>(height()) / num_rows;
}
