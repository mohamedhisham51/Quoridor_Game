#include "RandomStrategy.h"
#include "GameState.h"
#include <random>
#include <vector>

using namespace std;

Move RandomStrategy::selectMove(const GameState& state, int playerId) {
    vector<Move> moves = state.getValidMovesForPlayer(playerId);
    if (moves.empty()) {
        return Move();
    }

    random_device rd;
    mt19937 gen(rd());
    // pick uniformly from the legal move list.
    uniform_int_distribution<size_t> dist(0, moves.size() - 1);
    return moves[dist(gen)];
}
