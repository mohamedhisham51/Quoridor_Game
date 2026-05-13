#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "IPlayer.h"
#include "IAIStrategy.h"

class AIPlayer : public IPlayer {
private:
    IAIStrategy* strategy;
    int playerId;

public:
    AIPlayer(int id, IAIStrategy* strat = nullptr);

    Move getMove(const GameState& state) override;
    bool isHuman() const override;

    void setStrategy(IAIStrategy* strat);
    void setPlayerId(int id);
};

#endif
