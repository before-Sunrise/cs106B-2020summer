#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;


/* Returns a set of valid North, South, East, and West moves from the current
 * GridLocation.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> moves;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (abs(dr) == abs(dc)) {
                continue;
            }
            GridLocation potentialMove = {cur.row + dr, cur.col + dc};
            if (maze.inBounds(potentialMove) && maze[potentialMove]) {
                moves.add(potentialMove);
            }
        }
    }
    return moves;
}

/* Helper function for finding the solution to a maze using depth-first search.
 */
bool solveMazeHelper(Grid<bool>& maze, Stack<GridLocation>& path, GridLocation cur) {
    MazeGraphics::highlightPath(path, "blue");
    GridLocation exitLoc = {maze.numRows() - 1, maze.numCols() - 1};

    if (cur == exitLoc) {
        MazeGraphics::highlightPath(path, "green");
        return true;
    }

    Set<GridLocation> validNeighbors = generateValidMoves(maze, cur);
    for (GridLocation loc: validNeighbors) {
        path.push(loc);
        maze[loc] = false;
        if (solveMazeHelper(maze, path, loc)) {
            return true;
        }
        path.pop();
        maze[loc] = true;
    }
    return false;
}

/* Solve the maze using depth first search
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Grid<bool> mazeCopy = maze;
    GridLocation start = {0, 0};
    Stack<GridLocation> path = {start};
    maze[start] = false;
    if (solveMazeHelper(mazeCopy, path, start)) {
        return path;
    }
    error("No solution!");
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * However, there are various malformed inputs which it does not
 * correctly handle. At the very end of this assignment, you will
 * update this code to have complete error-checking behavior to make
 * the function fully robust. Then, update this header comment to
 * indicate which error checking cases you added. See the writeup
 * for more details.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            }
        }
    }
}
