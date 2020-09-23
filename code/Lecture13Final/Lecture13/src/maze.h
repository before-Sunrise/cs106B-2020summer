#pragma once

#include "stack.h"
#include "grid.h"
#include "set.h"

// Prototypes to be shared with other modules

Set<GridLocation> generateValidMoves(Grid<bool>& g, GridLocation cur);

void readMazeFile(std::string filename, Grid<bool>& maze);

Stack<GridLocation> solveMaze(Grid<bool>& maze);
