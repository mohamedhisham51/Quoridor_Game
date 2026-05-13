#ifndef MOVE_H
#define MOVE_H

#include "Enums.h"
#include "Position.h"
#include "Wall.h"

class Move {
public:
    MoveType type;
    Position target;
    Wall wall;
    int playerId;
    bool hasWall;

    Move();
    Move(int playerId, Position target);
    Move(int playerId, Wall wall);

    bool isWallMove() const;
};

#endif
