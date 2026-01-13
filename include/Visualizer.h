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

private:
    StateSimulation& simulation_;
    std::mutex& simulation_mutex_;
    
    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;
    static const QColor GRID_LINE_COLOR;
    
    static constexpr int DEFAULT_WINDOW_WIDTH = 800;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
    static constexpr int MIN_WINDOW_WIDTH = 400;
    static constexpr int MIN_WINDOW_HEIGHT = 300;
    
    void drawLawnGrid(QPainter& painter);
    
    double calculateFieldPixelWidth() const;
    double calculateFieldPixelHeight() const;
};
