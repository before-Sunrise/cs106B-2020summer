/*
 * CS 106B, Julie Zelenski
 *
 * This file declares a class named Ball.
 * A class is a template or blueprint for a new type of object.
 *
 * Each Ball object represents information about a ball.
 * A ball has a location (x,y), velocity (vx, vy) and id number.
 */
#pragma once
#include "gwindow.h"

class Ball {

  public:

    // constructor (initializes new object)

    // id number is used to label ball when drawn
    // The new ball is placed at random location of window
    // and set into motion with random velocity
    Ball(int id, GWindow* w);

    // member functions

    // Draw this ball as a numbered circle at its current location
    void draw();

    // Move this ball by one velocity increment
    void move();

    friend std::ostream& operator<<(std::ostream& out, const Ball& ball);

  private:
    // member variables
    // names prefixed with underscore to remind us these are member variables
    int x;
    int y;
    int vx;
    int vy;
    int id;
    GWindow* window;
};
