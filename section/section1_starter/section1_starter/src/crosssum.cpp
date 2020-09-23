/*
 * CS106B Section Handout Test Harness: Section 1
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
#include "grid.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * CrossSum (Code Write)
 * ----------------------------------
 * Given 3 parameters, a reference to a grid of integers and two integers for
 * a row and column, return the sum of all numbers in the row / col provided.
 *
 * You may assume row and col are within the bounds of the grid.
 * Do not modify the grid.
 */

int crossSum(const Grid<int>& g, int row, int col) {
    int sum = 0;
    for(int c = 0; c < g.numCols(); c++) sum += g[row][c];
    for(int r = 0; r < g.numRows(); r++) sum += g[r][col];
    sum -= g[row][col];
    return sum;
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Verifies our solution works on the handout's example case.") {
    Grid<int> g = { {1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9} };
    EXPECT_EQUAL(crossSum(g, 1, 1), 25);
}

PROVIDED_TEST("Provided Test: Verifies our solution works on a larger grid.") {
    Grid<int> g = { {1, 6, 5, 4, 2, 73, 2, 9, 0, -4, 24, 7},
                    {2, 43, 64, -53, 8, 4, 6, 78, 5, 1, 4, -6},
                    {7, 8, 9, 10, 11, -12, 5, 65, 4, 0,-6, 13},
                    {14, 52, 6, 8, 4, 2, -6, 5, 8, 3, 6, -33} };
    EXPECT_EQUAL(crossSum(g, 2, 8), 127);
}
