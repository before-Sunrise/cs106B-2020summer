/*
 * FILE: SNOWFLAKE FRACTALS
 * ---------------------
 * This program draws a Snowflake line of a constant depth on the screen.
 * A Snowflake line of depth N is 4 snowflake lines of depth N-1: two 1/3
 * of the length of the desired line (on the leftmost and rightmost sides),
 * and two forming a triangle in the middle 1/3 of the length of the
 * desired line.
 */

#include <math.h>
#include "gwindow.h"
#include "gobjects.h"
#include "point.h"
#include "fractals.h"

using namespace std;

/* Function: Draw Snowflake Line
 * ---------------------
 * A recursive function that draws a Snowflake line between points
 * "start" and "end." The fractal will have "level" numbers of levels.
 */
void drawSnowflakeLine(GWindow & window, int level, GPoint start, GPoint end) {
    if (level == 0) {
        window.drawLine(start, end);
    } else {
        GPoint a = pointBetween(start, end, 1.0 / 3);
        GPoint b = pointBetween(start, end, 2.0 / 3);
        GPoint t = thirdEquilateralPoint(a, b);

        // TIP: reverse the order of these recursive calls to draw the line from R to L!
        drawSnowflakeLine(window, level - 1, start, a);
        drawSnowflakeLine(window, level - 1, a, t);
        drawSnowflakeLine(window, level - 1, t, b);
        drawSnowflakeLine(window, level - 1, b, end);
    }
}

/* PROVIDED HELPER FUNCTIONS */

/* Method: Third Equilateral Point
 * -----------------------
 * Given the bottom left point of an equilateral triangle, and one other point
 * (either the top or the bottom right), returns the third point in the triangle.
 */
GPoint thirdEquilateralPoint(GPoint bottomLeft, GPoint otherPoint) {
    double deltaX = (otherPoint.getX() - bottomLeft.getX());
    double deltaY = (otherPoint.getY() - bottomLeft.getY());
    double x = bottomLeft.getX() + (deltaX*COS_60 + deltaY*SIN_60);
    double y = bottomLeft.getY() + (deltaY*COS_60 - deltaX*SIN_60);
    GPoint tip(x, y);
    return tip;
}

void runSnowflakeDemo() {
    GWindow w(SCREEN_WIDTH, SCREEN_HEIGHT);
    GPoint bottomLeft(BASE_LEFT_X, BASE_Y);
    GPoint bottomRight(BASE_RIGHT_X, BASE_Y);
    w.setColor("black");
    w.setFillColor("black");
    drawSnowflakeLine(w, FRACTAL_DEPTH, bottomLeft, bottomRight);

    // TIP: we can draw snowflake lines at any angle!  Try this to experiment
//    GPoint top = thirdEquilateralPoint(bottomLeft, bottomRight);
//    drawSnowflakeLine(w, FRACTAL_DEPTH, bottomLeft, top);
}

