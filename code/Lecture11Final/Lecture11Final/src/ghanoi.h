/*
 * File: ghanoi.h
 * --------------
 * This file defines the interface for the graphical Hanoi functions.
 */

#ifndef _ghanoi_h
#define _ghanoi_h

#include "gwindow.h"

/*
 * Function: initHanoiDisplay
 * Usage: initHanoiDisplay(n);
 * -------------------------------
 * Initializes the graphics window and draws the starting configuration
 * for a Towers of Hanoi puzzle for n disks.
 */

void initHanoiDisplay(int n);

/*
 * Function: moveSingleDisk
 * Usage: moveSingleDisk(char start, char finish);
 * -----------------------------------------------
 * Moves a single disk from the start tower to the finish
 * tower.  This method generates an error if the start tower
 * is empty or if the move violates the rules of the Hanoi
 * puzzle by placing a larger disk on top of a smaller one.
 */

void moveSingleDisk(char start, char finish);

#endif
