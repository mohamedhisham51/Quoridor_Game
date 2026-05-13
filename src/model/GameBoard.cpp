#include "GameBoard.h"
#include "PathFinder.h"
#include <iostream>

using namespace std;

GameBoard::GameBoard(int size) : boardSize(size) {
    reset();
}

void GameBoard::setSize(int size) {
    if (size < 2) {
        size = 2;
    } else if (size > 12) {
        size = 12;
    }
    boardSize = size;
    reset();
}

void GameBoard::reset() {
    walls.clear();
    cells.assign(boardSize, vector<Cell>(boardSize));
    for (int r = 0; r < boardSize; ++r) {
        for (int c = 0; c < boardSize; ++c) {
            cells[r][c] = Cell(r, c);
        }
    }
}

int GameBoard::getBoardSize() const {
    return boardSize;
}

const vector<Wall>& GameBoard::getWalls() const {
    return walls;
}

// check whether movement is blocked at this cell edge.
bool GameBoard::isBlocked(int row, int col, char direction) const {
    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) return true;

    if (direction == 'U') return cells[row][col].hasWallN;
    if (direction == 'D') return cells[row][col].hasWallS;
    if (direction == 'L') return cells[row][col].hasWallW;
    if (direction == 'R') return cells[row][col].hasWallE;
    
    return true; 
}

bool GameBoard::isValidMove(Position pos, char dir) const {
    int targetRow = pos.row;
    int targetCol = pos.col;

    // fail fast if a wall blocks the chosen direction.
    if (isBlocked(pos.row, pos.col, dir)) {
        return false;
    }

    // convert direction into target coordinates.
    if (dir == 'U') targetRow--;
    if (dir == 'D') targetRow++;
    if (dir == 'L') targetCol--;
    if (dir == 'R') targetCol++;

    // reject moves that leave board bounds.
    Position targetPos(targetRow, targetCol);
    if (!targetPos.isValid(boardSize)) {
        return false;
    }

    return true;
}

bool GameBoard::placeWall(Wall w) {
    // walls anchor between cells, so max valid index is boardSize - 2.
    if (w.row < 0 || w.row >= boardSize - 1 || w.col < 0 || w.col >= boardSize - 1) {
        return false;
    }

    // reject duplicate, crossing, or overlapping placements.
    for (const Wall& existingWall : walls) {
        if (existingWall.conflicts(w)) {
            return false; 
        }
    }

    // commit wall only after collision checks pass.
    walls.push_back(w);

    // mark both adjacent cell edges blocked by this wall.
    if (w.orientation == Orientation::HORIZONTAL) {
        // horizontal walls block south on top cells and north on bottom cells.
        cells[w.row][w.col].hasWallS = true;
        cells[w.row + 1][w.col].hasWallN = true;
        
        cells[w.row][w.col + 1].hasWallS = true;
        cells[w.row + 1][w.col + 1].hasWallN = true;
    } else {
        // vertical walls block east on left cells and west on right cells.
        cells[w.row][w.col].hasWallE = true;
        cells[w.row][w.col + 1].hasWallW = true;
        
        cells[w.row + 1][w.col].hasWallE = true;
        cells[w.row + 1][w.col + 1].hasWallW = true;
    }

    return true;
}

bool GameBoard::hasPath(Position pos, int goalRow) const {
    return PathFinder::bfs(*this, pos, goalRow);
}