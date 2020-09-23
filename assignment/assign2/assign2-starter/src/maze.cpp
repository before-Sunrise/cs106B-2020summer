/*
*this project focuses on a particular type of maze known as a perfect maze.
*A perfect, or simply-connected,maze has no loops and no inaccessible areas.
*Any two locations within a perfect maze are connected by exactly one path.
*/
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

//check whether current location is out of boundary
bool checkBoundary(GridLocation cur, int row, int col){ // Grid.inbounds has the same functionality
    if(cur.row < 0 || cur.row > row - 1 || cur.col < 0 || cur.col >  col -1){
        return false;
    }
    return true;
}

// move n/s/e/w from the current location
GridLocation nextLocation(GridLocation cur, int option){
    GridLocation next = cur;
    switch (option) {
        case 0: next.row--; break;
        case 1: next.row++; break;
        case 2: next.col--; break;
        case 3: next.col++; break;
    }
    return next;
}
/* generate valid Moves corresponding to the given maze and location cur;
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    for(int i = 0; i < 4; i++){
        GridLocation next = nextLocation(cur,i);
        if(checkBoundary(next, maze.height(), maze.width()) && maze[next]){ //这里行数和列数最开始传反了，导致debug很久
            neighbors.add(next);
        }
    }
    // cout << "generate valid moves:" << neighbors.toString() << endl;
    return neighbors;
}


/* for the  given path(bottom is start, top is the end)
 * and maze, check whether the path is the right solution to the maze
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    if(path.size() < maze.height() + maze.width() -1){ //最开始没加这一判断条件导致249行fail
        error("path is too short to succeed");
    }
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEnter = {0, 0};
    Set<GridLocation> visited;
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    GridLocation last = path.pop();
    visited.add(last);
    if(last == mazeEnter && path.isEmpty()) return; // maze is 1x1 size
    while(path.size() > 1){
        GridLocation current = path.peek();
        Set<GridLocation> validMoves = generateValidMoves(maze,last);
        // cout << "validMovies are " << validMoves.toString() << endl;
        if(!validMoves.contains(current)){
            error("current move is not legal from" + current.toString() + " to " + last.toString() + "\n");
        }

        if(visited.contains(current)){
             error("the current location " + current.toString() + "is visited before\n" + visited.toString());
        }

        last = path.pop();
        visited.add(last);
    }
    // now pathSize is 1
    GridLocation final = path.pop();
    if(final != mazeEnter || visited.contains(final)){
        error("path does not start at maze start or maze start is visited before\n path start at " + final.toString()
              + "visited set after the first step is "+ visited.toString());
    }

}

/* given a maze, return the path to go through the maze using BFS
 * same as what the word ladder did
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Stack<GridLocation> path{GridLocation{0,0}};
    Set<GridLocation> visited; // avoid repeated visit
    Queue<Stack<GridLocation>> queueOfPath;
    queueOfPath.enqueue(path);
    visited.add(path.peek());
    while(!queueOfPath.isEmpty()){
        Stack<GridLocation> currentPath = queueOfPath.dequeue();
        GridLocation endLocation = currentPath.peek();
        if(endLocation == mazeExit){
            MazeGraphics::highlightPath(currentPath, "red");
            return currentPath;
         }
        Set<GridLocation> neighbors = generateValidMoves(maze, endLocation);
        for(auto neighbor:neighbors){
            if(!visited.contains(neighbor)){//这一位置之前没有访问过
                visited.add(neighbor);
                Stack<GridLocation> nextPath = currentPath;
                nextPath.add(neighbor);
                queueOfPath.enqueue(nextPath);
            }
        }
    }
    return {};
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
    for(int i = 1; i < numRows; i++){
        if(lines[i].length() != numCols){
            error("every row should have the same length");
        }
    }
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            }else{
                error("charater should be @ or -");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}


PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}


STUDENT_TEST("generateNeighbors on corner of 2x2 grid with all walls"){
    Grid<bool> maze = {{false, false},
                       {false, false}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}
STUDENT_TEST("generateNeighbors on corner of 2x2 grid with no neighbor"){
    Grid<bool> maze = {{true, false},
                       {false, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

STUDENT_TEST("checkSolution on overboundary path should raise error"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> over_boundary = { {0, 0}, {0, 1},{0,2},{1,2},{1,1} };
    EXPECT_ERROR(checkSolution(maze, over_boundary));
}

STUDENT_TEST("checkSolution on path containing loop should raise error"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> pathWithLoop = { {0, 1}, {0, 1},{1,1},{1,0},{1,1} };
    EXPECT_ERROR(checkSolution(maze, pathWithLoop));
}

STUDENT_TEST("solveMaze on file 13x39"){
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/13x39.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 13x39"){
    Grid<bool> maze;
    readMazeFile("res/13x39.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/13x39.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}


STUDENT_TEST("solveMaze on file 17x37"){
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/17x37.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}


STUDENT_TEST("solveMaze on file 19x11"){
    Grid<bool> maze;
    readMazeFile("res/19x11.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/19x11.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 19x35"){
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/19x35.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 21x23"){
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/21x23.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 21x25"){
    Grid<bool> maze;
    readMazeFile("res/21x25.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/21x25.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 21x37"){
    Grid<bool> maze;
    readMazeFile("res/21x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/21x37.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 25x33"){
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/25x33.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 2x2"){
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/2x2.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}

STUDENT_TEST("solveMaze on file 5x7"){
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
    Stack<GridLocation> givenSoln;
    readSolutionFile("res/5x7.soln",givenSoln);
    EXPECT_EQUAL(givenSoln,soln);

}
