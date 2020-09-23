#include <iostream>
#include "console.h"
#include "strlib.h"
#include "prototypes.h"
#include "set.h"
#include "testing/SimpleTest.h"
#include "maze.h"
using namespace std;

int main() {
    if (runSimpleTests(SELECTED_TESTS)){
        return 0;
    }

    /*Set<string> teachingTeam = {"Nick", "Kylie", "Trip"};
    listSubsets(teachingTeam);*/


    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    solveMaze(maze);

    return 0;
}
