/*
 * CS106B Section Handout Test Harness: Section 8
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "testing/TestDriver.h"
using namespace std;

/*
 * Recursion Review (Code Writing)
 * --------------------------------------------------------
 * Imagine you have a 2 × n grid that you’d like to cover
 * using 2 × 1 dominoes. The dominoes need to be completely
 * contained within the grid (so they can’t hang over the
 * sides), can’t overlap, and have to be at 90° angles (so
 * you can’t have diagonal or tilted tiles). There’s exactly
 * one way to tile a 2 × 1 grid this way, exactly two ways
 * to tile a 2 × 2 grid this way, and exactly three ways to
 * tile a 2 × 3 grid this way (can you see what they are?)
 * Write a recursive function that, given a number n, returns
 * the number of ways you can tile a 2 × n grid with 2 × 1
 * dominoes.
 */

int numWaysToTile(int n) {
    if( n == 1) return 1;
    if( n == 2) return 2;
    return numWaysToTile(n - 1) + numWaysToTile(n - 2);
    return 0;
}

PROVIDED_TEST("Examples from handout for tiling board"){
    EXPECT_EQUAL(numWaysToTile(1), 1);
    EXPECT_EQUAL(numWaysToTile(2), 2);
    EXPECT_EQUAL(numWaysToTile(3), 3);
    EXPECT_EQUAL(numWaysToTile(4), 5);
    EXPECT_EQUAL(numWaysToTile(5), 8);
    EXPECT_EQUAL(numWaysToTile(6), 13);
    EXPECT_EQUAL(numWaysToTile(10), 89);
}
