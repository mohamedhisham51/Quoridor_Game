#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include "GameBoard.h"
#include "GameConfig.h"
#include "Move.h"
#include "Player.h"
#include "Enums.h"

using namespace std;

class GameState {
private:
    GameBoard board;
    vector<Player> players;
    int currentTurn;
    vector<Move> moveHistory;
    GameStatus status;
    GameConfig config;

    int indexForPlayerId(int playerId) const;
    vector<Position> getValidPawnMovesByIndex(int playerIndex) const;
    bool canPlaceWall(const Wall& wall, int playerIndex) const;
    bool applyMoveInternal(const Move& move, bool record);

public:
    GameState(GameConfig cfg = GameConfig());

    bool applyMove(const Move& move);
    bool undoMove();
    bool isGameOver() const;
    GameState* clone() const;

    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;
    const GameBoard& getBoard() const;
    GameBoard& getBoard();
    const vector<Player>& getPlayers() const;
    const GameConfig& getConfig() const;
    const vector<Move>& getHistory() const;

    vector<Move> getValidMovesForPlayer(int playerId) const;

    GameStatus getStatus() const;
    int getCurrentTurn() const;

    void reset();

    void setPlayer(int index, const Player& player);
    void setCurrentTurn(int turn);
    void setStatus(GameStatus newStatus);
    void clearHistory();
    void setHistory(const vector<Move>& history);
};

#endif
