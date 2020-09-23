#pragma once

#include "gwindow.h"
#include "gobjects.h"
#include "point.h"

/* Cantor Set Information */
GPoint getLoweredPoint(GPoint point);
void drawThickLine(GWindow &w, GPoint left, GPoint right);
void runCantorDemo();

// display constants
static const int FRACTAL_DEPTH = 6;
static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = SCREEN_WIDTH;
static const int LEVEL_HEIGHT = 20;
static const int RECT_HEIGHT = 10;
static const int INSET = 20;

/* Snowflake Information */

GPoint pointBetween(GPoint p1, GPoint p2, double fraction);
GPoint thirdEquilateralPoint(GPoint bottomLeft, GPoint otherPoint);
void runSnowflakeDemo();

// useful math constants
static const double COS_60 = 0.5;            //value of cos(60 degrees)
static const double SIN_60 = sqrt(3)*0.5;    //value of sin(60 degrees)

// display constants
//static const int FRACTAL_DEPTH = 3;
//static const int SCREEN_WIDTH = 1000;
//static const int SCREEN_HEIGHT = SCREEN_WIDTH;
static const int BASE_Y = SCREEN_HEIGHT - SCREEN_HEIGHT * .4;
static const int BASE_LEFT_X = 170;
static const int BASE_RIGHT_X = SCREEN_WIDTH - 170;

/* Sierpinski Carpet Information */
void runSierpinskiDemo();

/* Constants controlling the window size. */
const double kWindowWidth  = 1000;
const double kWindowHeight = 800;

/* Margin from window sides to content area. */
const double kMargin = 40;

/* Colors! */
const std::string kBackgroundColor = "white";
const std::string kRectangleColor  = "#400080";

/* Maximum order to draw. */
const double kMaxOrder = 5;

void clear(GWindow& window);
void repaint(GWindow& window);

