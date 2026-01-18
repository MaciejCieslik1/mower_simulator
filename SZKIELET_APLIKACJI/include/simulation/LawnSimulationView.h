/* 
    Author: Hanna Biegacz, Maciej Cieślik
    
    Displays the lawn on screen and shows which parts have been mowed.
    Green color means grass is still there, brown means it has been cut.
    
    Updates the display regularly (default 60 times per second) using a timer.
    Safely reads lawn data even when the simulation thread is changing it.
*/

#pragma once

#include <QWidget>
#include <QColor>
#include <QTimer>
#include <mutex>

class Lawn;

class LawnSimulationView : public QWidget {
    Q_OBJECT

public:
    explicit LawnSimulationView(Lawn* lawn, std::mutex& lawn_mutex, QWidget* parent = nullptr);
    ~LawnSimulationView();
    
    LawnSimulationView(const LawnSimulationView&) = delete;
    LawnSimulationView& operator=(const LawnSimulationView&) = delete;
    
    void refresh();
    void startSimulation(const int fps = 60);
    void stopSimulation();
    
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onTimerUpdate();

private:
    Lawn* lawn_;
    std::mutex& lawn_mutex_;
    QTimer* update_timer_;
    
    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;
    static const QColor GRID_LINE_COLOR;
    
    static constexpr int DEFAULT_WINDOW_WIDTH = 800;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
    static constexpr int MIN_WINDOW_WIDTH = 400;
    static constexpr int MIN_WINDOW_HEIGHT = 300;
    
    void drawLawnGrid(QPainter& painter);
    void drawFieldAt(QPainter& painter, const unsigned int row, const unsigned int col, const bool is_mowed);
    
    double calculateFieldPixelWidth() const;
    double calculateFieldPixelHeight() const;
};
