/*
 * CS 106B/X, Marty Stepp
 * This file implements the GUI for the Towers of hanoi problem.
 */

#include "hanoigui.h"
#include <string>
#include "error.h"
#include "gobjects.h"
#include "stack.h"
#include "strlib.h"
#include "vector.h"
#include "map.h"

namespace HanoiGui {

static GWindow* window = nullptr;
static const int NUM_PEGS = 3;
static int DELAY_MS = 200;
static Vector<Vector<int>> discs;
static const Vector<string> DISC_COLORS {
    "red", "green", "yellow", "magenta", "cyan", "orange", "gray", "brown"
};
static Map<string, int> pegs {{"left", 0}, {"middle", 1}, {"right", 2}};


static bool OO_DRAW = true;
static bool WAIT_FOR_CLICK = false;

static void draw() {
    window->clearCanvas();

    // draw 3 pegs
    window->setColor("black");
    double pegX = 100;
    double pegY = 40;
    double pegWidth = 20;
    double pegHeight = window->getHeight() - 60;
    double pegGap = 150;

    for (int i = 0; i < NUM_PEGS; i++) {
        if (OO_DRAW) {
            GRect* rect = new GRect(pegX - pegWidth / 2, pegY, pegWidth, pegHeight);
            rect->setFilled(true);
            rect->setFillColor("black");
            window->add(rect);
        } else {
            window->setColor("black");
            window->fillRect(pegX - pegWidth / 2, pegY, pegWidth, pegHeight);
        }
        pegX += pegGap;
    }

    // draw discs
    double discX = 100;
    for (int i = 0; i < NUM_PEGS; i++) {
        double discWidthBase = 30;
        double discWidthInc = 20;
        double discHeight = 30;
        double discY = pegY + pegHeight - discHeight;
        for (int discNum : discs[i]) {
            string color = DISC_COLORS[discNum % DISC_COLORS.size()];

            double discWidth = discWidthBase + discNum * discWidthInc;

            if (OO_DRAW) {
                GRect* rect = new GRect(discX - discWidth / 2, discY, discWidth, discHeight);
                rect->setFilled(true);
                rect->setFillColor(color);
                window->add(rect);
            } else {
                window->setColor(color);
                window->fillRect(discX - discWidth / 2, discY, discWidth, discHeight);
            }

            discY -= discHeight;
        }
        discX += pegGap;
    }
    window->repaint();

    // pause for animation
    if (WAIT_FOR_CLICK) {
        waitForClick();
    } else {
        pause(DELAY_MS);
    }
}

void initialize(int numDiscs) {
    if (!window) {
        window = new GWindow(550, 300);
        window->setLocation(0, 0);
        window->setTitle("CS 106B/X Towers of Hanoi");
        window->setResizable(false);
        GObject::setAntiAliasing(false);
        // window->setCanvasDrawingMode(GWindow::DRAW_OBJECT_ORIENTED);
        window->setRepaintImmediately(false);
    }

    discs.clear(); // clear any previous contents

    // set up 3 empty pegs
    for (int i = 0; i < NUM_PEGS; i++) {
        discs.add(Vector<int>());
    }

    // all discs on first peg
    for (int i = numDiscs; i >= 1; i--) {
        discs[0].add(i);
    }

    draw();
    window->show();
}

void moveOneDisc(int startPeg, int endPeg) {
    // check for various kinds of errors
    startPeg = isalpha(startPeg) ? startPeg - 'A' : startPeg;
    endPeg = isalpha(endPeg) ? endPeg - 'A' : endPeg;
    if (startPeg < 0 || startPeg > NUM_PEGS) {
        error("Invalid start peg: " + integerToString(startPeg));
    }
    if (endPeg < 0 || endPeg > NUM_PEGS) {
        error("Invalid end peg: " + integerToString(endPeg));
    }
    if (discs[startPeg].isEmpty()) {
        error("Peg " + integerToString(startPeg) + " has no discs");
    }
    if (!discs[endPeg].isEmpty() && discs[endPeg].back() < discs[startPeg].back()) {
        error("Disc on peg " + integerToString(startPeg)
              + " is too big to put onto peg " + integerToString(endPeg));
    }

    // no errors, so move the disc
    int disc = discs[startPeg].removeBack();
    discs[endPeg].add(disc);

    // repaint the graphics to show new state
    draw();
}

// overloaded version that refers to pegs by string name
void moveOneDisc(string startPeg, string endPeg) {
    moveOneDisc(pegs[startPeg], pegs[endPeg]);
}

void setDelay(int ms) {
    DELAY_MS = ms;
}

int thirdPegNumber(int peg1, int peg2) {
    return 3 - peg1 - peg2;
}

void setWaitForClick(bool wait) {
    WAIT_FOR_CLICK = wait;
}

} // end namespace
