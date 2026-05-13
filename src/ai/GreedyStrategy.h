#ifndef GREEDYSTRATEGY_H
#define GREEDYSTRATEGY_H

#include "IAIStrategy.h"

class GreedyStrategy : public IAIStrategy {
private:
    int heuristic(const GameState& state, int playerId) const;

public:
    Move selectMove(const GameState& state, int playerId) override;
};

#endif
