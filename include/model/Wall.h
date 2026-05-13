#ifndef WALL_H
#define WALL_H

#include "Enums.h"

class Wall {
public:
    int row;
    int col;
    Orientation orientation;

    Wall(int r = 0, int c = 0, Orientation o = Orientation::HORIZONTAL);

    // detect duplicate, crossing, or overlap conflict with another wall.
    bool conflicts(const Wall& other) const;
};

#endif