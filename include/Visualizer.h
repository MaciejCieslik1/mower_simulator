/* 
    Author: Hanna Biegacz
    
    Visualizer - displays the lawn and mowing progress on screen.
    TODO: update description
*/

#pragma once

#include <QWidget>
#include <QColor>
#include <QElapsedTimer>
#include "RenderContext.h"

class StateSimulation;
class Mower;

class Visualizer : public QWidget {
    Q_OBJECT

public:
    explicit Visualizer(RenderContext& render_context, QWidget* parent = nullptr);
    ~Visualizer();
    
    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;
    
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    // TODO: DELEGATE TASKS TO ANOTHER CLASS?
    static constexpr int DEFAULT_WINDOW_WIDTH = 1000;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 800;
    static constexpr int MIN_WINDOW_WIDTH = 400;            
    static constexpr int MIN_WINDOW_HEIGHT = 300;

    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;

    void loadMowerImage();
    void loadPointImages();
    void setupPainter(QPainter& painter);

    void updateSmoothedRenderTime();
    void refreshStateAndLayout();
    void updateLayout();
    bool hasSignificantTimeDrift(double actual_sim_time) const;

    void renderLawn(QPainter& painter);
    void renderMower(QPainter& painter, const SimulationSnapshot& sim_snapshot);
    void renderPoints(QPainter& painter);

    QPointF mapToScreen(double x_cm, double y_cm);
    void calculateMowerRenderSize(double mower_width, double mower_length, double blade_diameter, double& out_w_px, double& out_h_px) const;
    bool hasValidLawnDimensions() const;
    bool isLawnDataEmpty() const;

    RenderContext& render_context_;

    SimulationSnapshot current_sim_snapshot_;
    
    QElapsedTimer frame_timer_;
    double smoothed_render_time_ = 0;

    QPixmap mower_image_;
    std::vector<QPixmap> point_pixmaps_;

    double scale_factor_ = 1.0;
    double lawn_length_cm_ = 0;
    QPointF map_offset_;

};
