/* 
    Author: [Your Name]
    
    LawnVisualizer is responsible for rendering the current state of a Lawn object
    using Qt graphics. It displays mowed fields in brown and unmowed fields in green.
*/
#pragma once

#include <QWidget>
#include <QColor>
#include <memory>

class Lawn;  

class LawnVisualizer : public QWidget {
    Q_OBJECT

public:
    explicit LawnVisualizer(Lawn* lawn, QWidget* parent = nullptr);
    
    LawnVisualizer(const LawnVisualizer&) = delete;
    LawnVisualizer& operator=(const LawnVisualizer&) = delete;
    
    void refresh();
    
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Lawn* lawn_;
    
    static const QColor UNMOWED_GRASS_COLOR;
    static const QColor MOWED_GRASS_COLOR;
    static const QColor GRID_LINE_COLOR;
    
    static constexpr int DEFAULT_WINDOW_WIDTH = 800;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 600;
    static constexpr int MIN_WINDOW_WIDTH = 400;
    static constexpr int MIN_WINDOW_HEIGHT = 300;
    
    void drawLawnGrid(QPainter& painter);
    void drawFieldAt(QPainter& painter, unsigned int row, unsigned int col, bool is_mowed);
    
    double calculateFieldPixelWidth() const;
    double calculateFieldPixelHeight() const;
};
