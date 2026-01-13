/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    Displays the lawn on screen and shows which parts have been mowed.
    Green color means grass is still there, brown means it has been cut.
    
    Safely reads lawn data from StateSimulation even when the simulation thread is changing it.
    Updates are triggered externally via triggerRepaint().
*/

#pragma once

#include <QWidget>
#include <QColor>
#include <mutex>

class StateSimulation;

class LawnSimulationView : public QWidget {
    Q_OBJECT

public:
    explicit LawnSimulationView(StateSimulation& simulation, std::mutex& simulation_mutex, QWidget* parent = nullptr);
    ~LawnSimulationView();
    
    LawnSimulationView(const LawnSimulationView&) = delete;
    LawnSimulationView& operator=(const LawnSimulationView&) = delete;
    
    // Updates the view. Thread-safe trigger for repaint.
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
