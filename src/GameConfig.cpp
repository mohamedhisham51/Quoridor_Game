#include "GameConfig.h"

#include <algorithm>

GameConfig::GameConfig(int size, int walls, Difficulty diff, GameMode mode)
  : boardSize(std::max(5, std::min(size, 12))),
      wallsPerPlayer(walls),
      difficulty(diff),
      mode(mode) {
    // clamp board size into supported range.
}
