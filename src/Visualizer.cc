/* 
    Author: Hanna Biegacz
    
    Visualizer implementation. 
*/
#include "Visualizer.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mover.h"
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QCoreApplication>
#include <iostream>
#include <mutex>
#include <cmath>
#include <algorithm>
#include <QMetaObject>

using namespace std;

const QColor Visualizer::UNMOWED_GRASS_COLOR = QColor(119, 221, 118);  
const QColor Visualizer::MOWED_GRASS_COLOR = QColor(152, 118, 85);     

Visualizer::Visualizer(StateSimulation& simulation, mutex& simulation_mutex, QWidget* parent)
    : QWidget(parent), simulation_(simulation), simulation_mutex_(simulation_mutex), scale_factor_(1.0) {
    
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    loadAssets();
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

void Visualizer::loadAssets() {
    if (!mower_image_.load("include/assets/mower_3.png")) {
        cerr << "Failed to load mower image from include/assets/mower_3.png" << endl;
    }
}

void Visualizer::recalculateLayout() {
    lock_guard<mutex> lock(simulation_mutex_);
    const Lawn& lawn = simulation_.getLawn();
    double lawn_width_cm = lawn.getWidth();
    double lawn_length_cm = lawn.getLength();
    
    if (lawn_width_cm <= 0 || lawn_length_cm <= 0) return;

    scale_factor_ = min(static_cast<double>(width()) / lawn_width_cm, 
                        static_cast<double>(height()) / lawn_length_cm);
    
    double x = (width() - (lawn_width_cm * scale_factor_)) / 2;
    double y = (height() - (lawn_length_cm * scale_factor_)) / 2;
    map_offset_ = QPointF(x, y);
}

void Visualizer::resizeEvent(QResizeEvent* event) {
    recalculateLayout();
    QWidget::resizeEvent(event);
    update();
}

QPointF Visualizer::worldToScreen(double x_cm, double y_cm) {
    double lawn_length_cm = simulation_.getLawn().getLength();
    double screen_x = map_offset_.x() + (x_cm * scale_factor_);
    double screen_y = map_offset_.y() + (lawn_length_cm * scale_factor_) - (y_cm * scale_factor_);
    
    return QPointF(screen_x, screen_y);
}

void Visualizer::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    lock_guard<mutex> lock(simulation_mutex_);
    
    drawLawnGrid(painter);
    drawMower(painter);
}

void Visualizer::drawLawnGrid(QPainter& painter) {
    const Lawn& lawn = simulation_.getLawn();
    const auto fields = lawn.getFields();
    
    if (fields.empty() || fields[0].empty()) return;

    const unsigned int num_rows = fields.size();
    const unsigned int num_cols = fields[0].size();
    
    const double field_width_cm = static_cast<double>(lawn.getWidth()) / num_cols;
    const double field_height_cm = static_cast<double>(lawn.getLength()) / num_rows;
    
    for (unsigned int row = 0; row < num_rows; ++row) {
        for (unsigned int col = 0; col < num_cols; ++col) {
            const bool is_mowed = fields[row][col];
            QColor fill_color = is_mowed ? MOWED_GRASS_COLOR : UNMOWED_GRASS_COLOR;
            
            double world_x = col * field_width_cm;
            double world_y_top = (row + 1) * field_height_cm;
            
            QPointF top_left = worldToScreen(world_x, world_y_top);
            
            double w_px = field_width_cm * scale_factor_;
            double h_px = field_height_cm * scale_factor_;
            
            painter.fillRect(QRectF(top_left.x(), top_left.y(), w_px, h_px), fill_color);
        }
    }
}

void Visualizer::drawMower(QPainter& painter) {
    const Mover& mover = simulation_.getMover();
    double x = mover.getX();
    double y = mover.getY();
    double orientation = mover.getAngle();
    double width_cm = mover.getWidth();
    double length_cm = mover.getLength();
    
    painter.save();
    
    QPointF center_pos = worldToScreen(x, y);
    painter.translate(center_pos);
    painter.rotate(-orientation);
    
    double mower_w_px = width_cm * scale_factor_;
    double mower_h_px = length_cm * scale_factor_;
    
    painter.drawPixmap(QRectF(-mower_w_px / 2.0, -mower_h_px / 2.0, mower_w_px, mower_h_px), mower_image_, mower_image_.rect());
    
    painter.restore();
}
