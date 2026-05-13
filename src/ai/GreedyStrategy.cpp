#include "GreedyStrategy.h"
#include "GameState.h"
#include "PathFinder.h"
#include <limits>
#include <vector>

using namespace std;

int GreedyStrategy::heuristic(const GameState& state, int playerId) const {
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

    // unreachable states get a large penalty distance.
    if (myDist < 0) {
        myDist = 1000;
    }
    if (oppDist < 0) {
        oppDist = 1000;
    }

    int wallDiff = players[myIndex].getWallsLeft() - players[oppIndex].getWallsLeft();
    return (oppDist - myDist) + wallDiff;
}

Move GreedyStrategy::selectMove(const GameState& state, int playerId) {
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
        // score each legal successor and keep the best one.
        int score = heuristic(copy, playerId);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}
