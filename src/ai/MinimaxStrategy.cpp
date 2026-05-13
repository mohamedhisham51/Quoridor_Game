#include "MinimaxStrategy.h"
#include "GameState.h"
#include "PathFinder.h"
#include <limits>
#include <vector>

using namespace std;

MinimaxStrategy::MinimaxStrategy(int depth) : depth(depth) {}

int MinimaxStrategy::evaluate(const GameState& state, int playerId) const {
    const vector<Player>& players = state.getPlayers();
    if (players.size() != 2) {
        return 0;
    }

    // map player id to local and opponent indices.
    int myIndex = (players[0].getId() == playerId) ? 0 : 1;
    int oppIndex = 1 - myIndex;

    int myDist = PathFinder::shortestPath(state.getBoard(),
                                          players[myIndex].getPosition(),
                                          players[myIndex].getGoalRow());
    int oppDist = PathFinder::shortestPath(state.getBoard(),
                                           players[oppIndex].getPosition(),
                                           players[oppIndex].getGoalRow());

    if (myDist < 0) {
        myDist = 1000;
    }
    if (oppDist < 0) {
        oppDist = 1000;
    }

    return (oppDist - myDist);
}

int MinimaxStrategy::minimax(GameState& state, int depth, int alpha, int beta, int playerId) const {
    // stop search at depth limit or terminal states.
    if (depth == 0 || state.isGameOver()) {
        return evaluate(state, playerId);
    }

    int currentPlayerId = state.getCurrentPlayer().getId();
    vector<Move> moves = state.getValidMovesForPlayer(currentPlayerId);
    if (moves.empty()) {
        return evaluate(state, playerId);
    }

    bool maximizing = (currentPlayerId == playerId);
    // choose max for self turns and min for opponent turns.
    int bestValue = maximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();

    for (const Move& move : moves) {
        GameState copy = state;
        if (!copy.applyMove(move)) {
            continue;
        }
        int value = minimax(copy, depth - 1, alpha, beta, playerId);

        if (maximizing) {
            bestValue = max(bestValue, value);
            alpha = max(alpha, value);
        } else {
            bestValue = min(bestValue, value);
            beta = min(beta, value);
        }

        if (beta <= alpha) {
            // alpha-beta cutoff: remaining moves cannot improve result.
            break;
        }
    }

    return bestValue;
}

Move MinimaxStrategy::selectMove(const GameState& state, int playerId) {
    vector<Move> moves = state.getValidMovesForPlayer(playerId);
    if (moves.empty()) {
        return Move();
    }

    int bestScore = numeric_limits<int>::min();
    Move bestMove = moves[0];

    for (const Move& move : moves) {
        GameState copy = state;
        if (!copy.applyMove(move)) {
            continue;
        }
        int score = minimax(copy, depth - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), playerId);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}
