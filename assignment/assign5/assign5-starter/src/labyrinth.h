#pragma once
#include "testing/MemoryDiagnostics.h"
#include <string>
#include "set.h"

/**
 * Type representing a cell in the maze.
 */
struct MazeCell {
    std::string contents;

    MazeCell* north;
    MazeCell* south;
    MazeCell* east;
    MazeCell* west;
};

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to find all items in the needed set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
bool isPathToFreedom(MazeCell* start, std::string path, Set<std::string> needs);

