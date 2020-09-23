/**
 * SierpinskiGUI.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith revision used quarter 19-2
 * minor changes for Spring 20202 juliez
 */
#include "SierpinskiGUI.h"
#include "GUIUtils.h"
#include "GVector.h"
#include "../recursion.h"
using namespace std;

namespace {
    const string kBackgroundColor = "#FFFFFF";

    /* Slider options. */
    const int kLowOrder     = 0;
    const int kHighOrder    = 8;
    const int kDefaultOrder = 0;

    /* Header information. */
    const string kHeader =
        "Use the slider at window bottom to change the order of the "
        "triangle. Drag the red dots to reposition the corners.";
    const string kHeaderFont      = "Serif-ITALIC-16";
    const string kHeaderFontColor = "#36454F"; // Charcoal
    const double kHeaderPadding   = 10;

    /* Control points. */
    const string kControlPointFillColor    = "#C41E3A"; // Cardinal
    const string kControlPointBorderColor  = "#620F1C"; // Half cardinal
    const double kControlPointRadius  = 10;
    const double kControlPointPadding = 25;

    /* Returns the center point of a GOval. */
    GPoint centerOf(const GOval* oval) {
        return {
            oval->getX() + oval->getWidth()  / 2.0,
            oval->getY() + oval->getHeight() / 2.0
        };
    }
}

SierpinskiGUI::SierpinskiGUI(GWindow& window) {
    mOrderSlider = Temporary<GSlider>(new GSlider(kLowOrder, kHighOrder, kDefaultOrder),
                                      window,
                                      "SOUTH");
    mOrderSlider->setPaintLabels(true);
    mOrderSlider->setPaintTicks(true);
    mOrderSlider->setSnapToTicks(true);
    mOrderSlider->setMajorTickSpacing(1);
    mOrderSlider->setBounds(0, 0, window.getCanvasWidth()-kControlPointPadding, mOrderSlider->getHeight());

    mOrder = kDefaultOrder;

    /* Set up the control points. First, see how much space we'll need to draw the header. */
    auto text = TextRender::construct(kHeader, {0, 0, window.getCanvasWidth(), window.getCanvasHeight()
                                      }, kHeaderFontColor, kHeaderFont);
    text->alignCenterHorizontally();
    text->draw(window);
    auto headerArea = text->computedBounds();

    mWorkspace = {kControlPointPadding, headerArea.getY() + headerArea.getHeight() + kControlPointPadding,
                        window.getCanvasWidth() - 2*kControlPointPadding,
                        window.getCanvasHeight() - 2*kControlPointPadding - headerArea.getY() - headerArea.getHeight()};


    /* Set the control points to the window corners. */
    GPoint corners[3];
    corners[0] = { mWorkspace.getX(),                             mWorkspace.getY() + mWorkspace.getHeight() };
    corners[1] = { mWorkspace.getX() + mWorkspace.getWidth(),     mWorkspace.getY() + mWorkspace.getHeight() };
    corners[2] = { mWorkspace.getX() + mWorkspace.getWidth() / 2, mWorkspace.getY()                      };

    for (size_t i = 0; i < 3; i++) {
        mCornerPoints[i] = new GOval(corners[i].getX() - kControlPointRadius,
                                     corners[i].getY() - kControlPointRadius,
                                     2 * kControlPointRadius,
                                     2 * kControlPointRadius);
        mCornerPoints[i]->setFilled(true);
        mCornerPoints[i]->setColor(kControlPointBorderColor);
        mCornerPoints[i]->setFillColor(kControlPointFillColor);
        window.add(mCornerPoints[i]);
    }
}

string SierpinskiGUI::name() {
    return "Sierpinski Triangle";
}

void SierpinskiGUI::changeOccurredIn(GObservable* source) {
    if (source == mOrderSlider) {
        /* See if the value changed. If so, redraw things. */
        int order = mOrderSlider->getValue();
        if (order != mOrder) {
            mOrder = order;
            requestRepaint();
        }
    }
}

void SierpinskiGUI::repaint(GWindow& window) {
    /* We only need to draw the header text once. */
    clearDisplay(window, kBackgroundColor);
   auto text = TextRender::construct(kHeader, {0, 0, window.getCanvasWidth(), window.getCanvasHeight()
                                     }, kHeaderFontColor, kHeaderFont);
   text->alignCenterHorizontally();
   text->draw(window);


    /* Redraw the triangle. */
    auto p0 = centerOf(mCornerPoints[0]);
    auto p1 = centerOf(mCornerPoints[1]);
    auto p2 = centerOf(mCornerPoints[2]);
    drawSierpinskiTriangle(window, p0, p1, p2,  mOrder);
}

void SierpinskiGUI::mousePressed(double x, double y) {
    /* See if we hit anything. */
    GPoint mouse = clampToWorkspace(x, y);
    for (auto* oval: mCornerPoints) {
        if (magnitudeOf(centerOf(oval) - mouse) <= kControlPointRadius) {
            mSelected = oval;
            mLastMouse = mouse;
            return;
        }
    }

    /* Oops - didn't hit anything. */
    mSelected = nullptr;
}

void SierpinskiGUI::mouseDragged(double x, double y) {
    /* If nothing is selected, there's nothing to do. */
    if (mSelected == nullptr) return;

    /* Shift the selected oval over by the delta between our current and last
     * point.
     */
    GPoint mouse = clampToWorkspace(x, y);
    auto delta = mouse - mLastMouse;

    mSelected->move(delta.x, delta.y);
    requestRepaint();

    mLastMouse = mouse;
}

GPoint SierpinskiGUI::clampToWorkspace(double x, double y) const {
    x = max(min(x, mWorkspace.getX() + mWorkspace.getWidth()),  mWorkspace.getX());
    y = max(min(y, mWorkspace.getY() + mWorkspace.getHeight()), mWorkspace.getY());

    return { x, y };
}

shared_ptr<ProblemHandler> makeInteractiveSierpinskiGUI(GWindow& window) {
    return make_shared<SierpinskiGUI>(window);
}
