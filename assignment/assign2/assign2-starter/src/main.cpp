#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
#include "search.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

int main()
{
    if (runSimpleTests(NO_TESTS)) {
        return 0;
    }

    /*Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    solveMaze(maze);*/


    searchEngine("res/website.txt");

    cout << endl << "All done!" << endl;
    return 0;
}
