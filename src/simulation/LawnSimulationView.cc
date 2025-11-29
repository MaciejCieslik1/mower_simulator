#include "simulation/LawnSimulationView.h"
#include "../../include/Lawn.h"
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <mutex>

using namespace std;

const QColor LawnSimulationView::UNMOWED_GRASS_COLOR = QColor(34, 139, 34);
const QColor LawnSimulationView::MOWED_GRASS_COLOR = QColor(139, 90, 43);
const QColor LawnSimulationView::GRID_LINE_COLOR = QColor(200, 200, 200, 100);

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

LawnSimulationView::~LawnSimulationView() {
    if (update_timer_) {
        update_timer_->stop();
        delete update_timer_;
        update_timer_ = nullptr;
    }
}

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

void LawnSimulationView::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);
    
    drawLawnGrid(painter);
}

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

double LawnSimulationView::calculateFieldPixelWidth() const {
    const auto fields = lawn_->getFields();
    if (fields.empty() || fields[0].empty()) {
        return 0.0;
    }
    
    const unsigned int num_cols = fields[0].size();
    return static_cast<double>(width()) / num_cols;
}

double LawnSimulationView::calculateFieldPixelHeight() const {
    const auto fields = lawn_->getFields();
    if (fields.empty()) {
        return 0.0;
    }
    
    const unsigned int num_rows = fields.size();
    return static_cast<double>(height()) / num_rows;
}
