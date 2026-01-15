/* 
    Author: Hanna Biegacz
    
    Visualizer implementation. 
    TODO: update description
*/
#include "Visualizer.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mover.h"
#include "MathHelper.h"
#include "Engine.h"
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


const QColor Visualizer::UNMOWED_GRASS_COLOR = QColor(75, 187, 103);  
const QColor Visualizer::MOWED_GRASS_COLOR =   QColor(115, 213, 139);

Visualizer::Visualizer(Engine& engine, QWidget* parent)
    : QWidget(parent), engine_(engine), scale_factor_(1.0) {
    // TODO: REMOVE MAGIC NUMBER
    current_snapshot_ = engine_.getRenderContext().getInterpolatedState(0);

    
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    loadImages();
}

Visualizer::~Visualizer() {
}


QSize Visualizer::sizeHint() const {
    return QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

QSize Visualizer::minimumSizeHint() const {
    return QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

void Visualizer::loadImages() {
    string assets_path = string(ASSETS_PATH);
    string mower_path = assets_path + "/mower.png";
    if (!mower_image_.load(mower_path.c_str())) {
        cerr << "[Visualizer] Failed to load mower image from file: " << mower_path << endl;
    }

    vector<string> point_colors = {
        "blue", "green", "navy", "orange", "pink", "purple", "yellow"
    };

    for (const string& color : point_colors) {
        string path = assets_path + "/point_" + color + ".png";
        QPixmap pixmap;
        if (pixmap.load(path.c_str())) {
            point_pixmaps_.push_back(pixmap);
        } else {
            cerr << "[Visualizer] Failed to load point image: " << path << endl;
        }
    }
}

void Visualizer::updateLayout() {
    double lawn_width_cm = current_snapshot_.lawn_width_;
    lawn_length_cm_ = current_snapshot_.lawn_length_;
    
    if (lawn_width_cm <= 0 || lawn_length_cm_ <= 0) return;

    scale_factor_ = min(static_cast<double>(width()) / lawn_width_cm, 
                        static_cast<double>(height()) / lawn_length_cm_);
    
    double x = (width() - (lawn_width_cm * scale_factor_)) / 2;
    double y = (height() - (lawn_length_cm_ * scale_factor_)) / 2;
    map_offset_ = QPointF(x, y);
}

void Visualizer::resizeEvent(QResizeEvent* event) {
    updateLayout();
    QWidget::resizeEvent(event);
    update();
}

QPointF Visualizer::mapToScreen(double x_cm, double y_cm) {
    double screen_x = map_offset_.x() + (x_cm * scale_factor_);
    double screen_y = map_offset_.y() + (lawn_length_cm_ * scale_factor_) - (y_cm * scale_factor_);
    
    return QPointF(screen_x, screen_y);
}

void Visualizer::paintEvent(QPaintEvent* event) {
    // TODO: REMOVE MAGIC NUMBERS
    // TODO: SPLIT INTO SMALLER FUNCTIONS
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    if (!frame_timer_.isValid()) {
        frame_timer_.start();
        smoothed_render_time_ = current_snapshot_.simulation_time_;
    }

    double dt_ms = static_cast<double>(frame_timer_.restart());
    double simulation_dt = dt_ms * engine_.getSpeedMultiplier();

    double actual_sim_time = engine_.getSimulationTime();
    
    if (std::abs(actual_sim_time - smoothed_render_time_) > 500.0) {
        smoothed_render_time_ = actual_sim_time;
    } else {
        smoothed_render_time_ += simulation_dt;
    }

    current_snapshot_ = engine_.getRenderContext().getInterpolatedState(smoothed_render_time_);
    updateLayout();

    renderLawn(painter);
    renderPoints(painter);
    renderMower(painter, current_snapshot_);
}

void Visualizer::renderLawn(QPainter& painter) {
    const auto& fields = current_snapshot_.fields_;
    
    if (fields.empty() || fields[0].empty()) return;

    const unsigned int num_rows = fields.size();
    const unsigned int num_cols = fields[0].size();
    
    const double field_width_cm = static_cast<double>(current_snapshot_.lawn_width_) / num_cols;
    const double field_height_cm = static_cast<double>(current_snapshot_.lawn_length_) / num_rows;

    
    for (unsigned int row = 0; row < num_rows; ++row) {
        for (unsigned int col = 0; col < num_cols; ++col) {
            const bool is_mowed = fields[row][col];
            QColor fill_color = is_mowed ? MOWED_GRASS_COLOR : UNMOWED_GRASS_COLOR;
            
            double world_x = col * field_width_cm;
            double world_y_top = (row + 1) * field_height_cm;
            
            QPointF top_left = mapToScreen(world_x, world_y_top);
            
            double w_px = field_width_cm * scale_factor_;
            double h_px = field_height_cm * scale_factor_;
            
            painter.fillRect(QRectF(top_left.x(), top_left.y(), w_px, h_px), fill_color);
        }
    }
}

void Visualizer::calculateMowerRenderSize(double mover_width, double mover_length, double blade_diameter, double& out_w_px, double& out_h_px) const {
    double display_width_cm = std::max(mover_width, blade_diameter);
    
    double scale_ratio = display_width_cm / mover_width;
    double display_length_cm = mover_length * scale_ratio;

    out_w_px = display_width_cm * scale_factor_;
    out_h_px = display_length_cm * scale_factor_;
}

void Visualizer::renderMower(QPainter& painter, const Snapshot& snapshot) {
    double mower_w_px, mower_h_px;
    calculateMowerRenderSize(snapshot.mower_width_, snapshot.mower_length_, snapshot.blade_diameter_, mower_w_px, mower_h_px);
    //TODO: CHANGE TO mowerRENDERsize
    
    painter.save();

    QPointF center_pos = mapToScreen(snapshot.x_, snapshot.y_);
    painter.translate(center_pos);
    
    painter.rotate(MathHelper::convertRadiansToDegrees(-snapshot.angle_));
    
    QRectF target_rect(-mower_w_px / 2.0, -mower_h_px / 2.0, mower_w_px, mower_h_px);
    
    painter.drawPixmap(target_rect, mower_image_, mower_image_.rect());
    
    painter.restore();
}

void Visualizer::renderPoints(QPainter& painter) {
    double MIN_ICON_HEIGHT = 30.0;
    double ICON_PROPORTION = 0.05;
    const auto& points = current_snapshot_.points_;

    double icon_height = height() * ICON_PROPORTION;
    
    if (icon_height < MIN_ICON_HEIGHT) icon_height = MIN_ICON_HEIGHT;

    for (size_t i = 0; i < points.size() && i < point_pixmaps_.size(); ++i) {
        const auto& point = points[i];
        const auto& pixmap = point_pixmaps_[i];
        
        QPointF screen_pos = mapToScreen(point.getX(), point.getY());
        
        double aspect_ratio = static_cast<double>(pixmap.width()) / (pixmap.height() > 0 ? pixmap.height() : 1);
        double icon_width = icon_height * aspect_ratio;
        
        QRectF target_rect(screen_pos.x() - icon_width / 2.0, screen_pos.y() - icon_height, icon_width, icon_height);
        
        painter.drawPixmap(target_rect, pixmap, pixmap.rect());
    }
}
