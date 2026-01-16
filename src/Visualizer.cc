/* 
    Author: Hanna Biegacz
    
    Visualizer implementation. 
*/
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QCoreApplication>
#include <QMetaObject>
#include <iostream>
#include "Visualizer.h"
#include "StateSimulation.h"
#include "Lawn.h"
#include "Mower.h"
#include "MathHelper.h"


using namespace std;


const QColor Visualizer::UNMOWED_GRASS_COLOR = QColor(75, 187, 103);  
const QColor Visualizer::MOWED_GRASS_COLOR =   QColor(115, 213, 139);

Visualizer::Visualizer(StateInterpolator& render_context, QWidget* parent)
    : QWidget(parent), state_interpolator_(render_context), render_time_controller_(render_context) { 
    current_sim_snapshot_ = state_interpolator_.getInterpolatedState(0);
    static_simulation_data = state_interpolator_.getStaticSimulationData();

    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    loadMowerImage();
    loadPointImages();
}

Visualizer::~Visualizer() {
}


QSize Visualizer::sizeHint() const {
    return QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

QSize Visualizer::minimumSizeHint() const {
    return QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

void Visualizer::loadMowerImage() {
    string assets_path = string(ASSETS_PATH);
    string mower_path = assets_path + "/mower.png";
    if (!mower_image_.load(mower_path.c_str())) {
        cerr << "[Visualizer] Failed to load mower image from file: " << mower_path << endl;
    }
}

void Visualizer::loadPointImages() {
    string assets_path = string(ASSETS_PATH);
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
    double lawn_width_cm = static_simulation_data.lawn_width_;
    lawn_length_cm_ = static_simulation_data.lawn_length_;
    
    if (!hasValidLawnDimensions()) return;

    scale_factor_ = min(static_cast<double>(width()) / lawn_width_cm, 
                        static_cast<double>(height()) / lawn_length_cm_);
    
    double x = (width() - (lawn_width_cm * scale_factor_)) / 2;
    double y = (height() - (static_simulation_data.lawn_length_ * scale_factor_)) / 2;
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
    QPainter painter(this);
    setupPainter(painter);
    
    updateRenderTime();
    refreshStateAndLayout();

    renderLawn(painter);
    renderPoints(painter);
    renderMower(painter, current_sim_snapshot_);
    
    update(); 
}

void Visualizer::updateRenderTime() {
    double ms_since_last_frame = 0.0; 
    
    if (frame_timer_.isValid()) {
        ms_since_last_frame = static_cast<double>(frame_timer_.restart());
    } else {
        frame_timer_.start();
    }

    render_time_controller_.update(ms_since_last_frame);
}

void Visualizer::setupPainter(QPainter& painter) {
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
}

void Visualizer::refreshStateAndLayout() {
    double render_time = render_time_controller_.getSmoothedTime();
    current_sim_snapshot_ = state_interpolator_.getInterpolatedState(render_time);
    static_simulation_data = state_interpolator_.getStaticSimulationData();
    updateLayout();
}

bool Visualizer::hasValidLawnDimensions() const {
    return static_simulation_data.lawn_width_ > 0 && static_simulation_data.lawn_length_ > 0;
}

bool Visualizer::isLawnDataEmpty() const {
    const auto& fields = current_sim_snapshot_.fields_;
    return fields.empty() || fields[0].empty();
}

void Visualizer::renderLawn(QPainter& painter) {
    if (isLawnDataEmpty()) return;

    const auto& fields = current_sim_snapshot_.fields_;
    const unsigned int num_rows = fields.size();
    const unsigned int num_cols = fields[0].size();
    
    const double field_width_cm = static_cast<double>(static_simulation_data.lawn_width_) / num_cols;
    const double field_height_cm = static_cast<double>(static_simulation_data.lawn_length_) / num_rows;

    
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

void Visualizer::calculateMowerRenderSize(double mower_width, double mower_length, double blade_diameter, double& out_w_px, double& out_h_px) const {
    double display_width_cm = std::max(mower_width, blade_diameter);
    
    double scale_ratio = display_width_cm / mower_width;
    double display_length_cm = mower_length * scale_ratio;

    out_w_px = display_width_cm * scale_factor_;
    out_h_px = display_length_cm * scale_factor_;
}

void Visualizer::renderMower(QPainter& painter, const SimulationSnapshot& sim_snapshot) {
    double mower_w_px, mower_h_px;
    calculateMowerRenderSize(static_simulation_data.width_cm, static_simulation_data.length_cm, 
                            static_simulation_data.blade_diameter_cm, mower_w_px, mower_h_px);
    painter.save();

    QPointF center_pos = mapToScreen(sim_snapshot.x_, sim_snapshot.y_);
    painter.translate(center_pos);
    painter.rotate(MathHelper::convertRadiansToDegrees(-sim_snapshot.angle_));
    
    QRectF target_rect(-mower_w_px / 2.0, -mower_h_px / 2.0, mower_w_px, mower_h_px);
    
    painter.drawPixmap(target_rect, mower_image_, mower_image_.rect());
    painter.restore();
}

void Visualizer::renderPoints(QPainter& painter) {
    double MIN_POINT_HEIGHT = 30.0;
    double POINT_PROPORTION = 0.05;
    const auto& points = current_sim_snapshot_.points_;

    double point_height = height() * POINT_PROPORTION;
    
    if (point_height < MIN_POINT_HEIGHT) point_height = MIN_POINT_HEIGHT;

    for (size_t i = 0; i < points.size() && i < point_pixmaps_.size(); ++i) {
        const auto& point = points[i];
        const auto& pixmap = point_pixmaps_[i];
        
        QPointF screen_pos = mapToScreen(point.getX(), point.getY());
        
        double aspect_ratio = static_cast<double>(pixmap.width()) / (pixmap.height() > 0 ? pixmap.height() : 1);
        double point_width = point_height * aspect_ratio;
        
        QRectF target_rect(screen_pos.x() - point_width / 2.0, screen_pos.y() - point_height, point_width, point_height);
        
        painter.drawPixmap(target_rect, pixmap, pixmap.rect());
    }
}
