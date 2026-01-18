#include <gtest/gtest.h>
#include <QApplication>
#include "../include/Visualizer.h"
#include "../include/StateInterpolator.h"

// Helper to ensure QApplication exists for QWidget creation
int argc = 0;
char *argv[] = {};
QApplication* app = nullptr;

void ensureApplication() {
    if (!QApplication::instance()) {
        app = new QApplication(argc, argv);
    }
}

TEST(VisualizerTest, constructorInitializeCorrectly) {
    ensureApplication();
    StateInterpolator interpolator;
    Visualizer visualizer(interpolator);

    EXPECT_TRUE(visualizer.minimumSizeHint().isValid());
    EXPECT_TRUE(visualizer.sizeHint().isValid());
}

TEST(VisualizerTest, defaultSizeHintsAreSensible) {
    ensureApplication();
    StateInterpolator interpolator;
    Visualizer visualizer(interpolator);

    QSize minSize = visualizer.minimumSizeHint();
    QSize preferredSize = visualizer.sizeHint();

    EXPECT_GE(minSize.width(), 0);
    EXPECT_GE(minSize.height(), 0);
    EXPECT_GE(preferredSize.width(), minSize.width());
    EXPECT_GE(preferredSize.height(), minSize.height());
}

TEST(VisualizerTest, canCreateAndDestroyWithoutCrash) {
    ensureApplication();
    StateInterpolator interpolator;
    
    {
        Visualizer visualizer(interpolator);
    }
    
    EXPECT_TRUE(true); // Should reach here
}
