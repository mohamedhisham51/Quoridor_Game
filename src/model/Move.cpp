#include "Move.h"

Move::Move()
    : type(MoveType::PAWN_MOVE),
      target(0, 0),
      wall(0, 0, Orientation::HORIZONTAL),
      playerId(-1),
      hasWall(false) {}

Move::Move(int playerId, Position target)
    : type(MoveType::PAWN_MOVE),
      target(target),
      wall(0, 0, Orientation::HORIZONTAL),
      playerId(playerId),
      hasWall(false) {}

Move::Move(int playerId, Wall wall)
    : type(MoveType::WALL_PLACE),
      target(0, 0),
      wall(wall),
      playerId(playerId),
      hasWall(true) {}

bool Move::isWallMove() const {
    // keep legacy flag check for backward compatibility with saves.
    return type == MoveType::WALL_PLACE || hasWall;
}
