#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "IPlayer.h"

class HumanPlayer : public IPlayer {
private:
    Move nextMove;
    bool hasMove;

public:
    HumanPlayer();
    Move getMove(const GameState& state) override;
    bool isHuman() const override;

    void setNextMove(const Move& move);
};

#endif
