/*
 * CS 106B, Julie Zelenski
 *
 * This file provides the implementation for the Ball class
 * constructor and member functions declared in ball.h.
 */
#include "ball.h"
#include "random.h"
#include "testing/SimpleTest.h"
using namespace std;

// constants used by the Ball class
const int SIZE = 30;
const int MIN_VELOCITY = -5;
const int MAX_VELOCITY = 5;


Ball::Ball(int id, GWindow* w) {
    window = w;
    this->id = id;
    //这里去除了bug，原版本为window->getWidth()
    x = randomInteger(0, window->getWidth() - SIZE);
    //同上
    y = randomInteger(0, window->getHeight() - SIZE);
    vx = randomInteger(MIN_VELOCITY, MAX_VELOCITY);
    vy = randomInteger(MIN_VELOCITY, MAX_VELOCITY);

}

// Draw this ball as a colored circle at its current location
// Label the ball with its id number
void Ball::draw() {
    window->setColor("Blue");
    window->fillOval(x, y, SIZE, SIZE);
    window->setColor("White");
    window->setFont("Helvetica-" + integerToString(SIZE/2));
    window->drawString(integerToString(id), x + SIZE/4, y + 3*SIZE/4);
}

// Change ball's location by one velocity increment in x and y directions
// If ball crosses edge of window, "bounce" by reflecting the velocity
// in x or y direction
void Ball::move() {
    if((x >570 &&vx >0) || (y > 370 && vy > 0)){
        cout << this;
    }
    x += vx;
    y += vy;
    // if outside left or right edge, bounce horizontally
    if (x < 0 || x + SIZE > window->getWidth()) {
        vx = -vx;

    }
    // if outside top or bottom edge, bounce vertically
    if (y < 0 || y + SIZE > window->getHeight()) {
        vy = -vy;

    }

}

std::ostream& operator<<(std::ostream& out, const Ball& ball){
    out << ball.id << " " << ball.x << " " << ball.vx
        << ball.y << " " << ball.vy << endl;
    return out;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Animate bouncing balls in window for a while")
{
    GWindow window;
    window.setTitle("Bouncing Balls");
    window.setLocation(0, 0);
    window.setCanvasSize(600, 400);
    window.setResizable(false);
    window.setAutoRepaint(false);

    // Construct many random ball objects, store all balls in a vector
    Vector<Ball> allBalls;
    for (int i = 0; i < 25; i++) {
         Ball ball(i, &window);
         allBalls.add(ball);
    }


    // animation loop: move/draw all balls in each time step
    for (int i = 0; i < 300; i++) {
        window.clearCanvas();
        for (int i = 0; i < allBalls.size(); i++) {
            allBalls[i].move();
            allBalls[i].draw();
        }
        window.repaint();
        pause(10); // very brief pause
    }
    window.close();
}
