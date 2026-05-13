#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "GameBoard.h"
#include "Position.h"
#include <queue>
#include <vector>

using namespace std;

class PathFinder {
public:
    static bool bfs(const GameBoard& board, Position start, int goalRow);
    static int shortestPath(const GameBoard& board, Position start, int goalRow);
};

#endif
