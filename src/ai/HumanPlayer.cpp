#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() : nextMove(), hasMove(false) {}

Move HumanPlayer::getMove(const GameState& state) {
    (void)state;
    if (hasMove) {
        // consume one queued input move.
        hasMove = false;
        return nextMove;
    }
    return Move();
}

bool HumanPlayer::isHuman() const {
    return true;
}

void HumanPlayer::setNextMove(const Move& move) {
    nextMove = move;
    hasMove = true;
}
