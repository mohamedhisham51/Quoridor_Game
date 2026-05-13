#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>

using namespace std;
#include "Cell.h"
#include "Wall.h"
#include "Position.h"

class GameBoard {
private:
    vector<vector<Cell>> cells;
    int boardSize;
    vector<Wall> walls;

public:
    explicit GameBoard(int size = 9);

    void setSize(int size);

    // core board operations used by game flow and ai checks.
    bool isValidMove(Position pos, char dir) const;
    bool placeWall(Wall w);
    bool hasPath(Position pos, int goalRow) const;
    void reset();
    int getBoardSize() const;
    const vector<Wall>& getWalls() const;
    
    // helper: check whether a wall blocks movement from this cell edge.
    bool isBlocked(int row, int col, char direction) const;
};

#endif