#ifndef IPLAYER_H
#define IPLAYER_H

#include "Move.h"

class GameState;

class IPlayer {
public:
    virtual ~IPlayer() {}
    virtual Move getMove(const GameState& state) = 0;
    virtual bool isHuman() const = 0;
};

#endif
