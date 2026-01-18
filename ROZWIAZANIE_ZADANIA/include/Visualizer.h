/* 
    Author: Hanna Biegacz
    
    This class handles the visualization layer using Qt.
    It draws the lawn, the mower, and the points based on the current simulation state.
    It connects the data from StateInterpolator with the timing from RenderTimeController.
    It also calculates scaling to fit the simulation world inside the application window.
*/

#pragma once

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QPixmap>
#include <vector>
#include "RenderTimeController.h"
#include "StateInterpolator.h"
#include "SimulationSnapshot.h"

class Visualizer : public QWidget {
    Q_OBJECT

public:
    explicit Visualizer(StateInterpolator& render_context, QWidget* parent = nullptr);
    ~Visualizer() override;
    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    static const int DEFAULT_WINDOW_WIDTH = 800;
    static const int DEFAULT_WINDOW_HEIGHT = 600;
    static const int MIN_WINDOW_WIDTH = 400;
    static const int MIN_WINDOW_HEIGHT = 300;
    
    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;

    StateInterpolator& state_interpolator_;
    SimulationSnapshot current_sim_snapshot_;
    RenderTimeController render_time_controller_;
    StaticSimulationData static_simulation_data_;
    std::vector<QPixmap> point_pixmaps_;
    QPixmap mower_image_;
    QPointF map_offset_;
    QElapsedTimer frame_timer_;
    double scale_factor_ = 1.0;
    double lawn_length_cm_ = 0.0;

    void updateRenderTime();
    void refreshStateAndLayout();
    void setupPainter(QPainter& painter);
    void updateLayout();
    void loadMowerImage();
    void loadPointImages();
    void renderLawn(QPainter& painter) const;
    void renderMower(QPainter& painter, const SimulationSnapshot& sim_snapshot) const;
    void renderPoints(QPainter& painter) const;
    QPointF mapToScreen(double x_cm, double y_cm) const;

    bool hasValidLawnDimensions() const;
    bool isLawnDataEmpty() const;
    void calculateMowerRenderSize(double mower_width, double mower_length, double blade_diameter, double& out_w_px, double& out_h_px) const;
};