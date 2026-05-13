#ifndef MINIMAXSTRATEGY_H
#define MINIMAXSTRATEGY_H

#include "IAIStrategy.h"

class MinimaxStrategy : public IAIStrategy {
private:
    int depth;

    int evaluate(const GameState& state, int playerId) const;
    int minimax(GameState& state, int depth, int alpha, int beta, int playerId) const;

public:
    MinimaxStrategy(int depth = 2);
    Move selectMove(const GameState& state, int playerId) override;
};

#endif
