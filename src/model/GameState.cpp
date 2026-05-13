#include "GameState.h"
#include "PathFinder.h"
#include <algorithm>

GameState::GameState(GameConfig cfg)
    : board(cfg.boardSize), currentTurn(0), status(GameStatus::IN_PROGRESS), config(cfg) {
    reset();
}

int GameState::indexForPlayerId(int playerId) const {
    if (players.size() != 2) {
        return -1;
    }
    if (players[0].getId() == playerId) {
        return 0;
    }
    if (players[1].getId() == playerId) {
        return 1;
    }
    return -1;
}

void GameState::reset() {
    board.setSize(config.boardSize);
    players.clear();

    int size = board.getBoardSize();
    int startCol = size / 2;

    // place both pawns in the center column on opposite edges.
    players.push_back(Player(1, "Player 1", Position(0, startCol), size - 1, config.wallsPerPlayer));
    players.push_back(Player(2, "Player 2", Position(size - 1, startCol), 0, config.wallsPerPlayer));

    currentTurn = 0;
    status = GameStatus::IN_PROGRESS;
    moveHistory.clear();
}

vector<Position> GameState::getValidPawnMovesByIndex(int playerIndex) const {
    vector<Position> moves;
    if (playerIndex < 0 || playerIndex >= static_cast<int>(players.size())) {
        return moves;
    }

    Position pos = players[playerIndex].getPosition();
    Position opp = players[1 - playerIndex].getPosition();

    const char dirs[4] = {'U', 'D', 'L', 'R'};
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        if (!board.isValidMove(pos, dirs[i])) {
            continue;
        }

        Position next(pos.row + dr[i], pos.col + dc[i]);
        if (next == opp) {
            if (board.isValidMove(opp, dirs[i])) {
                // jump straight when the cell behind opponent is open.
                Position jump(opp.row + dr[i], opp.col + dc[i]);
                if (jump.isValid(board.getBoardSize())) {
                    moves.push_back(jump);
                }
            } else {
                // if straight jump is blocked, allow side-step diagonals.
                if (dirs[i] == 'U' || dirs[i] == 'D') {
                    if (board.isValidMove(opp, 'L')) {
                        Position diag(opp.row, opp.col - 1);
                        if (diag.isValid(board.getBoardSize())) {
                            moves.push_back(diag);
                        }
                    }
                    if (board.isValidMove(opp, 'R')) {
                        Position diag(opp.row, opp.col + 1);
                        if (diag.isValid(board.getBoardSize())) {
                            moves.push_back(diag);
                        }
                    }
                } else {
                    if (board.isValidMove(opp, 'U')) {
                        Position diag(opp.row - 1, opp.col);
                        if (diag.isValid(board.getBoardSize())) {
                            moves.push_back(diag);
                        }
                    }
                    if (board.isValidMove(opp, 'D')) {
                        Position diag(opp.row + 1, opp.col);
                        if (diag.isValid(board.getBoardSize())) {
                            moves.push_back(diag);
                        }
                    }
                }
            }
        } else {
            moves.push_back(next);
        }
    }

    return moves;
}

bool GameState::canPlaceWall(const Wall& wall, int playerIndex) const {
    if (playerIndex < 0 || playerIndex >= static_cast<int>(players.size())) {
        return false;
    }
    if (players[playerIndex].getWallsLeft() <= 0) {
        return false;
    }

    GameBoard temp = board;
    if (!temp.placeWall(wall)) {
        return false;
    }

    // each player must still have at least one path after placement.
    for (const Player& player : players) {
        if (!PathFinder::bfs(temp, player.getPosition(), player.getGoalRow())) {
            return false;
        }
    }

    return true;
}

bool GameState::applyMoveInternal(const Move& move, bool record) {
    if (status != GameStatus::IN_PROGRESS) {
        return false;
    }

    int playerIndex = indexForPlayerId(move.playerId);
    // reject out-of-turn and unknown-player moves.
    if (playerIndex == -1 || playerIndex != currentTurn) {
        return false;
    }

    if (move.isWallMove()) {
        // walls must be legal globally and locally before mutating state.
        if (!canPlaceWall(move.wall, playerIndex)) {
            return false;
        }
        if (!board.placeWall(move.wall)) {
            return false;
        }
        if (!players[playerIndex].useWall()) {
            return false;
        }
    } else {
        vector<Position> validMoves = getValidPawnMovesByIndex(playerIndex);
        bool found = false;
        for (const Position& pos : validMoves) {
            if (pos == move.target) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
        // commit pawn move only after validation against generated moves.
        players[playerIndex].move(move.target);
        if (players[playerIndex].hasReachedGoal()) {
            status = (playerIndex == 0) ? GameStatus::PLAYER1_WIN : GameStatus::PLAYER2_WIN;
        }
    }

    if (record) {
        // keep history aligned with successful state mutations only.
        moveHistory.push_back(move);
    }

    if (status == GameStatus::IN_PROGRESS) {
        // switch turns unless a winning move ended the game.
        currentTurn = 1 - currentTurn;
    }

    return true;
}

bool GameState::applyMove(const Move& move) {
    return applyMoveInternal(move, true);
}

bool GameState::undoMove() {
    if (moveHistory.empty()) {
        return false;
    }

    vector<Move> history = moveHistory;
    history.pop_back();

    // rebuild deterministic state from history minus the last move.
    reset();
    for (const Move& move : history) {
        if (!applyMoveInternal(move, true)) {
            return false;
        }
    }

    return true;
}

bool GameState::isGameOver() const {
    return status != GameStatus::IN_PROGRESS;
}

GameState* GameState::clone() const {
    return new GameState(*this);
}

Player& GameState::getCurrentPlayer() {
    return players[currentTurn];
}

const Player& GameState::getCurrentPlayer() const {
    return players[currentTurn];
}

const GameBoard& GameState::getBoard() const {
    return board;
}

GameBoard& GameState::getBoard() {
    return board;
}

const vector<Player>& GameState::getPlayers() const {
    return players;
}

const GameConfig& GameState::getConfig() const {
    return config;
}

const vector<Move>& GameState::getHistory() const {
    return moveHistory;
}

vector<Move> GameState::getValidMovesForPlayer(int playerId) const {
    vector<Move> moves;
    int playerIndex = indexForPlayerId(playerId);
    if (playerIndex == -1) {
        return moves;
    }

    vector<Position> pawnMoves = getValidPawnMovesByIndex(playerIndex);
    for (const Position& pos : pawnMoves) {
        moves.push_back(Move(playerId, pos));
    }

    if (players[playerIndex].getWallsLeft() > 0) {
        int size = board.getBoardSize();
        // try each wall anchor in both orientations.
        for (int r = 0; r < size - 1; ++r) {
            for (int c = 0; c < size - 1; ++c) {
                Wall h(r, c, Orientation::HORIZONTAL);
                if (canPlaceWall(h, playerIndex)) {
                    moves.push_back(Move(playerId, h));
                }
                Wall v(r, c, Orientation::VERTICAL);
                if (canPlaceWall(v, playerIndex)) {
                    moves.push_back(Move(playerId, v));
                }
            }
        }
    }

    return moves;
}

GameStatus GameState::getStatus() const {
    return status;
}

int GameState::getCurrentTurn() const {
    return currentTurn;
}

void GameState::setPlayer(int index, const Player& player) {
    if (index < 0 || index >= static_cast<int>(players.size())) {
        return;
    }
    players[index] = player;
}

void GameState::setCurrentTurn(int turn) {
    if (turn == 0 || turn == 1) {
        currentTurn = turn;
    }
}

void GameState::setStatus(GameStatus newStatus) {
    status = newStatus;
}

void GameState::clearHistory() {
    moveHistory.clear();
}

void GameState::setHistory(const vector<Move>& history) {
    moveHistory = history;
}
