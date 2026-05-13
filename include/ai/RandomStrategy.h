#ifndef RANDOMSTRATEGY_H
#define RANDOMSTRATEGY_H

#include "IAIStrategy.h"

class RandomStrategy : public IAIStrategy {
public:
    Move selectMove(const GameState& state, int playerId) override;
};

#endif
