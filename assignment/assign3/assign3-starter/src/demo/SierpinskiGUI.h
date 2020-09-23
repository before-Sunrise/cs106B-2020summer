#pragma once

#include "ProblemHandler.h"
#include "TemporaryComponent.h"
#include "ginteractors.h"
#include "gobjects.h"

/* Problem handler to visualize the Sierpinski triangles of different orders. */
class SierpinskiGUI: public ProblemHandler {
public:
    SierpinskiGUI(GWindow& window);

    void changeOccurredIn(GObservable* source) override;
    void mousePressed(double x, double y) override;
    void mouseDragged(double x, double y) override;

    static std::string name();

protected:
    void repaint(GWindow& window) override;

private:
    Temporary<GSlider> mOrderSlider;
    int  mOrder;

    /* Current corner points. */
    GOval* mCornerPoints[3];

    /* Last mouse pushdown point. */
    GPoint mLastMouse;

    /* Last selected oval. */
    GOval* mSelected = nullptr;

    /* Workspace area where the triangle can be displayed. */
    GRectangle mWorkspace;

    /* Given an {x, y} point, clamps it into the workspace. */
    GPoint clampToWorkspace(double x, double y) const;
};

