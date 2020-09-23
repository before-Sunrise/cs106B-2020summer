/*
 * CS 106B/X, Marty Stepp
 * This file declares the GUI for the Towers of hanoi problem.
 */

#include <iostream>
#include "gwindow.h"

using namespace std;

namespace HanoiGui {

/*
 * Sets up the GUI with the given number of discs on peg 0.
 */
void initialize(int numDiscs);

/*
 * Performs a move of the top disc from the given start peg
 * to the top of the given end peg.
 * If the peg indexes are invalid, or there is no disc on the start
 * peg, or if it is too big to put onto the end peg, an error occurs.
 */
void moveOneDisc(int startPeg, int endPeg);
void moveOneDisc(string startPeg, string endPeg);

/*
 * Helper that returns the peg number that is not either of the two
 * given peg numbers from 0-2 inclusive.
 * For example, thirdPegNumber(1, 0) is 2,
 *          and thirdPegNumber(1, 2) is 0.
 * Assumes that 0 <= peg1, peg2 <= 2.
 */
int thirdPegNumber(int peg1, int peg2);

/*
 * Sets an animation delay between moves, in milliseconds.
 */
void setDelay(int ms);

/*
 * Sets the program to pause indefinitely waiting for the mouse to be clicked
 * on the Hanoi GUI window after each move.
 */
void setWaitForClick(bool wait);

} // end namespace
