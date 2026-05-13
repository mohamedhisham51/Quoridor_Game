#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "Enums.h"

class GameConfig {
public:
    int boardSize;
    int wallsPerPlayer;
    Difficulty difficulty;
    GameMode mode;

    GameConfig(int size = 9,
               int walls = 10,
               Difficulty diff = Difficulty::EASY,
               GameMode mode = GameMode::HUMAN_VS_HUMAN);
};

#endif
