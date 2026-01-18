/* 
    Author: Hanna Biegacz
    
    LawnSimulationView implementation. 
*/
#include "simulation/LawnSimulationView.h"
#include "../../include/Lawn.h"
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <mutex>

using namespace std;

const QColor LawnSimulationView::UNMOWED_GRASS_COLOR = QColor(119, 221, 118);  
const QColor LawnSimulationView::MOWED_GRASS_COLOR = QColor(152, 118, 85);     
const QColor LawnSimulationView::GRID_LINE_COLOR = QColor(200, 200, 200, 100);

// Sets up the window. Checks if lawn pointer is valid (throws error if null).
// Sets minimum and default window sizes.
LawnSimulationView::LawnSimulationView(Lawn* lawn, mutex& lawn_mutex, QWidget* parent)
    : QWidget(parent), lawn_(lawn), lawn_mutex_(lawn_mutex), update_timer_(nullptr) {
    
    if (!lawn_) {
        cerr << "[LawnSimulationView] ERROR: Lawn pointer is null" << endl;
        throw invalid_argument("Lawn pointer cannot be null");
    }
    
    const auto fields = lawn_->getFields();
    
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    resize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

// Cleans up on exit. Stops and deletes timer if it exists.
LawnSimulationView::~LawnSimulationView() {
    if (update_timer_) {
        update_timer_->stop();
        delete update_timer_;
        update_timer_ = nullptr;
    }
}

// Starts periodic screen updates at given FPS (frames per second).
// Creates QTimer and connects it to onTimerUpdate slot.
// Calculates update interval in milliseconds (1000/fps). Validates fps value.
void LawnSimulationView::startSimulation(const int fps) {
    if (update_timer_) {
        return;
    }
    
    if (fps <= 0) {
        cerr << "[LawnSimulationView] Invalid FPS value: " << fps << endl;
        return;
    }
    
    update_timer_ = new QTimer(this);
    connect(update_timer_, &QTimer::timeout, this, &LawnSimulationView::onTimerUpdate);
    
    const int interval_ms = 1000 / fps;
    update_timer_->start(interval_ms);
}

// Stops periodic screen updates. Stops timer and schedules it for deletion.
// Sets timer pointer to null.
void LawnSimulationView::stopSimulation() {
    if (!update_timer_) {
        return;
    }
    
    update_timer_->stop();
    update_timer_->deleteLater();
    update_timer_ = nullptr;
}

void LawnSimulationView::onTimerUpdate() {
    refresh();
}

void LawnSimulationView::refresh() {
    update();
}

QSize LawnSimulationView::sizeHint() const {
    return QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

QSize LawnSimulationView::minimumSizeHint() const {
    return QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

// Qt calls this automatically when window needs redrawing.
// Creates painter, disables antialiasing for sharp grid lines, then draws lawn.
void LawnSimulationView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    drawLawnGrid(painter);
}

// Draws entire lawn grid. Locks mutex for thread-safe access to lawn data.
// Calculates pixel size for each field, then loops through all fields.
// Colors each field green (unmowed) or brown (mowed). 
// Adds grid lines only if fields are big enough (2px+).
void LawnSimulationView::drawLawnGrid(QPainter& painter) {
    lock_guard<mutex> lock(lawn_mutex_);
    
    const auto fields = lawn_->getFields();
    
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

// Draws single field at given row/col position.
// Colors it green (unmowed) or brown (mowed) based on is_mowed parameter.
// Always draws grid border around the field.
void LawnSimulationView::drawFieldAt(QPainter& painter, const unsigned int row, const unsigned int col, const bool is_mowed) {
    const double field_width_px = calculateFieldPixelWidth();
    const double field_height_px = calculateFieldPixelHeight();
    
    const double x = col * field_width_px;
    const double y = height() - (row + 1) * field_height_px;
    
    const QColor fill_color = is_mowed ? MOWED_GRASS_COLOR : UNMOWED_GRASS_COLOR;
    
    painter.fillRect(QRectF(x, y, field_width_px, field_height_px), fill_color);
    painter.setPen(GRID_LINE_COLOR);
    painter.drawRect(QRectF(x, y, field_width_px, field_height_px));
}

// Returns how many pixels wide each lawn field should be.
// Divides window width by number of columns. Returns 0.0 if lawn is empty.
double LawnSimulationView::calculateFieldPixelWidth() const {
    const auto fields = lawn_->getFields();
    if (fields.empty() || fields[0].empty()) {
        return 0.0;
    }
    
    const unsigned int num_cols = fields[0].size();
    return 20.0* static_cast<double>(width()) / num_cols;
}

// Returns how many pixels tall each lawn field should be.
// Divides window height by number of rows. Returns 0.0 if lawn is empty.
double LawnSimulationView::calculateFieldPixelHeight() const {
    const auto fields = lawn_->getFields();
    if (fields.empty()) {
        return 0.0;
    }
    
    const unsigned int num_rows = fields.size();
    return 20.0* static_cast<double>(height()) / num_rows;
}
