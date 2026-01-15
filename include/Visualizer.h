/* 
    Author: Hanna Biegacz
    
    Visualizer - displays the lawn and mowing progress on screen.
    TODO: update description
*/

#pragma once

#include <QWidget>
#include <QColor>
#include <mutex>
#include <QElapsedTimer>
#include "RenderContext.h"

class StateSimulation;
class Mower;
class Engine;

class Visualizer : public QWidget {
    Q_OBJECT

public:
    explicit Visualizer(Engine& engine, QWidget* parent = nullptr);
    ~Visualizer();
    
    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;
    
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    // TODO: SORT 
    // TODO: DELEGATE TASKS TO ANOTHER CLASS?
    Engine& engine_;
    Snapshot current_snapshot_;
    QPixmap mower_image_;
    double scale_factor_= 1.0;
    QPointF map_offset_;
    double lawn_length_cm_ = 0;

    QElapsedTimer frame_timer_;
    double last_simulation_time_ = -1;
    double smoothed_render_time_ = 0;

    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;
    std::vector<QPixmap> point_pixmaps_;
    
    static constexpr int DEFAULT_WINDOW_WIDTH = 1000;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 800;
    static constexpr int MIN_WINDOW_WIDTH = 800;            //TODO: DO WE NEED THIS?
    static constexpr int MIN_WINDOW_HEIGHT = 600;
    
    void setupPainter(QPainter& painter);
    void updateSmoothedRenderTime();
    bool hasSignificantTimeDrift(double actual_sim_time) const;
    void refreshStateAndLayout();
    bool hasValidLawnDimensions() const;
    bool isLawnDataEmpty() const;
    void loadMowerImage();
    void loadPointImages();
    void updateLayout();
    QPointF mapToScreen(double x_cm, double y_cm);
    void renderLawn(QPainter& painter);
    void renderMower(QPainter& painter, const Snapshot& snapshot);
    void renderPoints(QPainter& painter);
    void calculateMowerRenderSize(double mower_width, double mower_length, double blade_diameter, double& out_w_px, double& out_h_px) const;

};
