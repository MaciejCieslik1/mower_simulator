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
    static_simulation_data_ = state_interpolator_.getStaticSimulationData();

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
        "0_purple", "1_pink", "2_red", "3_orange", "4_yellow", "5_green", "6_blue", "7_navy"
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

// Calculates how to fit the simulation lawn inside the window. Finds the scale factor
// (zoom level) that makes the lawn fit, and calculates the offset to center it.
// The lawn maintains its aspect ratio and is centered in the window.
void Visualizer::updateLayout() {
    double lawn_width_cm = static_simulation_data_.lawn_width_;
    lawn_length_cm_ = static_simulation_data_.lawn_length_;
    
    if (!hasValidLawnDimensions()) return;

    scale_factor_ = min(static_cast<double>(width()) / lawn_width_cm, 
                        static_cast<double>(height()) / lawn_length_cm_);
    
    double x = (width() - (lawn_width_cm * scale_factor_)) / 2;
    double y = (height() - (static_simulation_data_.lawn_length_ * scale_factor_)) / 2;
    map_offset_ = QPointF(x, y);
}

void Visualizer::resizeEvent(QResizeEvent* event) {
    updateLayout();
    QWidget::resizeEvent(event);
    update();
}

// Converts simulation coordinates (in centimeters) to screen coordinates (in pixels).
// Important: The simulation uses standard math coordinates (Y up), but screens use
// Y down, so we flip the Y axis. Also applies scaling and centering offset.
QPointF Visualizer::mapToScreen(double x_cm, double y_cm) const{
    double screen_x = map_offset_.x() + (x_cm * scale_factor_);
    double screen_y = map_offset_.y() + (lawn_length_cm_ * scale_factor_) - (y_cm * scale_factor_);
    
    return QPointF(screen_x, screen_y);
}

// Main rendering function called automatically by Qt every frame. Updates time,
// fetches the current interpolated state, and draws the lawn, points, and mower.
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

// Tracks time between frames using Qt's timer. On first run, starts the timer.
// On subsequent runs, restarts it and returns elapsed milliseconds.
// This time is used by RenderTimeController for smooth animation.
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

// Fetches the latest interpolated state for the current render time and
// updates layout in case window size or simulation data changed.
void Visualizer::refreshStateAndLayout() {
    double render_time = render_time_controller_.getSmoothedTime();
    current_sim_snapshot_ = state_interpolator_.getInterpolatedState(render_time);
    static_simulation_data_ = state_interpolator_.getStaticSimulationData();
    updateLayout();
}

bool Visualizer::hasValidLawnDimensions() const {
    return static_simulation_data_.lawn_width_ > 0 && static_simulation_data_.lawn_length_ > 0;
}

bool Visualizer::isLawnDataEmpty() const {
    const auto& fields = current_sim_snapshot_.fields_;
    return fields.empty() || fields[0].empty();
}

// Draws the lawn by creating a QImage from the boolean grid (mowed vs unmowed).
// Each cell in the simulation grid becomes one pixel in the image. The image is then
// stretched to fit the screen using the calculated scale. Antialiasing is temporarily
// disabled to keep grass cells sharp and prevent blending between mowed/unmowed areas.
void Visualizer::renderLawn(QPainter& painter) const {
    if (isLawnDataEmpty()) return;

    const auto& fields = current_sim_snapshot_.fields_;
    const int num_rows = static_cast<int>(fields.size());
    const int num_cols = static_cast<int>(fields[0].size());

    QImage lawn_image(num_cols, num_rows, QImage::Format_RGB32);
    
    for (int row = 0; row < num_rows; ++row) {
        int img_row = num_rows - 1 - row;
        const auto& field_row = fields[row];
        for (int col = 0; col < num_cols; ++col) {
            lawn_image.setPixel(col, img_row, 
                field_row[col] ? MOWED_GRASS_COLOR.rgb() : UNMOWED_GRASS_COLOR.rgb());
        }
    }

    QPointF top_left_px = mapToScreen(0, static_simulation_data_.lawn_length_);
    double w_px = static_simulation_data_.lawn_width_ * scale_factor_;
    double h_px = static_simulation_data_.lawn_length_ * scale_factor_;
    QRectF target_rect(top_left_px.x(), top_left_px.y(), w_px, h_px);

    bool old_aa = painter.renderHints().testFlag(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    painter.drawImage(target_rect, lawn_image);
    
    painter.setRenderHint(QPainter::Antialiasing, old_aa);
}

// Calculates the display size of the mower in pixels. The mower should be at least
// as wide as its blade diameter for realistic representation so that the mowed path isn't wider than the mower.
void Visualizer::calculateMowerRenderSize(double mower_width, double mower_length, double blade_diameter, double& out_w_px, double& out_h_px) const {
    double display_width_cm = std::max(mower_width, blade_diameter);
    
    double scale_ratio = display_width_cm / mower_width;
    double display_length_cm = mower_length * scale_ratio;

    out_w_px = display_width_cm * scale_factor_;
    out_h_px = display_length_cm * scale_factor_;
}

void Visualizer::renderMower(QPainter& painter, const SimulationSnapshot& sim_snapshot) const {
    double mower_w_px, mower_h_px;
    calculateMowerRenderSize(static_simulation_data_.width_cm_, static_simulation_data_.length_cm, 
                            static_simulation_data_.blade_diameter_cm, mower_w_px, mower_h_px);
    painter.save();

    QPointF center_pos = mapToScreen(sim_snapshot.x_, sim_snapshot.y_);
    painter.translate(center_pos);
    painter.rotate(sim_snapshot.angle_);
    
    QRectF target_rect(-mower_w_px / 2.0, -mower_h_px / 2.0, mower_w_px, mower_h_px);
    
    painter.drawPixmap(target_rect, mower_image_, mower_image_.rect());
    painter.restore();
}

void Visualizer::renderPoints(QPainter& painter) const {
    double MIN_POINT_HEIGHT = 30.0;
    double POINT_PROPORTION = 0.05;
    const auto& points = current_sim_snapshot_.points_;

    double point_height = height() * POINT_PROPORTION;
    
    if (point_height < MIN_POINT_HEIGHT) point_height = MIN_POINT_HEIGHT;

    for (size_t i = 0; i < points.size(); ++i) {
        const auto& point = points[i];
        
        size_t image_index = i % point_pixmaps_.size();
        const auto& pixmap = point_pixmaps_[image_index];
        
        QPointF screen_pos = mapToScreen(point.getX(), point.getY());
        
        double aspect_ratio = static_cast<double>(pixmap.width()) / (pixmap.height() > 0 ? pixmap.height() : 1);
        double point_width = point_height * aspect_ratio;
        
        QRectF target_rect(screen_pos.x() - point_width / 2.0, screen_pos.y() - point_height, point_width, point_height);
        
        painter.drawPixmap(target_rect, pixmap, pixmap.rect());
    }
}
