#include "AIPlayer.h"

AIPlayer::AIPlayer(int id, IAIStrategy* strat) : strategy(strat), playerId(id) {}

Move AIPlayer::getMove(const GameState& state) {
    if (!strategy) {
        return Move();
    }
    // delegate move selection to the active ai strategy.
    return strategy->selectMove(state, playerId);
}

bool AIPlayer::isHuman() const {
    return false;
}

void AIPlayer::setStrategy(IAIStrategy* strat) {
    strategy = strat;
}

void AIPlayer::setPlayerId(int id) {
    playerId = id;
}
