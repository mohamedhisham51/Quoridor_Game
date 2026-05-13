#include "Cell.h"

// initialize cell coordinates and start with no blocking walls.
Cell::Cell(int r, int c) : row(r), col(c) {
    clearWalls();
}

void Cell::clearWalls() {
    hasWallN = false;
    hasWallE = false;
    hasWallS = false;
    hasWallW = false;
}