/* 
    Author: Hanna Biegacz
    
    Visualizer - displays the lawn and mowing progress on screen.
*/

#pragma once

#include <QWidget>
#include <QColor>
#include <mutex>

class StateSimulation;

class Visualizer : public QWidget {
    Q_OBJECT

public:
    explicit Visualizer(StateSimulation& simulation, std::mutex& simulation_mutex, QWidget* parent = nullptr);
    ~Visualizer();
    
    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;
    
    void triggerRepaint();
    
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    StateSimulation& simulation_;
    std::mutex& simulation_mutex_;
    QPixmap mower_image_;
    double scale_factor_;
    QPointF map_offset_;

    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;
    
    static constexpr int DEFAULT_WINDOW_WIDTH = 1000;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 800;
    static constexpr int MIN_WINDOW_WIDTH = 800;
    static constexpr int MIN_WINDOW_HEIGHT = 600;
    
    void loadAssets();
    void recalculateLayout();
    QPointF worldToScreen(double x_cm, double y_cm);
    void drawLawnGrid(QPainter& painter);
    void drawMower(QPainter& painter);


};
