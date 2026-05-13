#ifndef IAISTRATEGY_H
#define IAISTRATEGY_H

#include "Move.h"

class GameState;

class IAIStrategy {
public:
    virtual ~IAIStrategy() {}
    virtual Move selectMove(const GameState& state, int playerId) = 0;
};

#endif
