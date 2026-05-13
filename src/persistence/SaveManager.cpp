#include "SaveManager.h"
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

bool SaveManager::save(const GameState& state,
                       const vector<Move>& undoStack,
                       const vector<Move>& redoStack,
                       const string& file) {
    ofstream out(file);
    if (!out.is_open()) {
        return false;
    }

    // write complete snapshot including undo and redo stacks.
    out << serialize(state, undoStack, redoStack);
    return true;
}

SaveManager::SaveData SaveManager::load(const string& file) {
    ifstream in(file);
    if (!in.is_open()) {
        return SaveData();
    }

    stringstream buffer;
    buffer << in.rdbuf();
    // parse from an in-memory buffer to keep token reads simple.
    return deserialize(buffer.str());
}

static void writeMove(stringstream& ss, const Move& move) {
    if (move.isWallMove()) {
        ss << "W " << move.playerId << " " << move.wall.row << " " << move.wall.col
           << " " << static_cast<int>(move.wall.orientation) << "\n";
    } else {
        ss << "P " << move.playerId << " " << move.target.row << " " << move.target.col << "\n";
    }
}

static bool readMove(stringstream& ss, Move& outMove) {
    char type = '\0';
    if (!(ss >> type)) {
        return false;
    }
    if (type == 'P') {
        int playerId = 0;
        int row = 0;
        int col = 0;
        ss >> playerId >> row >> col;
        outMove = Move(playerId, Position(row, col));
        return true;
    }
    if (type == 'W') {
        int playerId = 0;
        int row = 0;
        int col = 0;
        int orientation = 0;
        ss >> playerId >> row >> col >> orientation;
        outMove = Move(playerId, Wall(row, col, static_cast<Orientation>(orientation)));
        return true;
    }
    return false;
}

string SaveManager::serialize(const GameState& state,
                              const vector<Move>& undoStack,
                              const vector<Move>& redoStack) {
    stringstream ss;
    // qdr2 extends qdr1 with undo and redo sections.
    ss << "QDR2\n";

    ss << state.getBoard().getBoardSize() << " "
       << state.getCurrentTurn() << " "
       << static_cast<int>(state.getStatus()) << "\n";

    const vector<Player>& players = state.getPlayers();
    ss << players.size() << "\n";
    for (const Player& player : players) {
        Position pos = player.getPosition();
        ss << player.getId() << " "
           << quoted(player.getName()) << " "
           << pos.row << " " << pos.col << " "
           << player.getWallsLeft() << " "
           << player.getGoalRow() << "\n";
    }

    const vector<Wall>& walls = state.getBoard().getWalls();
    ss << walls.size() << "\n";
    for (const Wall& wall : walls) {
        ss << wall.row << " " << wall.col << " " << static_cast<int>(wall.orientation) << "\n";
    }

    ss << undoStack.size() << "\n";
    for (const Move& move : undoStack) {
        writeMove(ss, move);
    }

    ss << redoStack.size() << "\n";
    for (const Move& move : redoStack) {
        writeMove(ss, move);
    }

    return ss.str();
}

SaveManager::SaveData SaveManager::deserialize(const string& data) {
    stringstream ss(data);
    string header;
    ss >> header;
    // accept both legacy and current save versions.
    if (header != "QDR1" && header != "QDR2") {
        return SaveData();
    }

    int size = 9;
    int currentTurn = 0;
    int status = 0;
    ss >> size >> currentTurn >> status;

    GameConfig cfg(size, 10, Difficulty::EASY, GameMode::HUMAN_VS_HUMAN);
    GameState state(cfg);

    size_t playerCount = 0;
    ss >> playerCount;
    // restore players in fixed index order expected by game state.
    for (size_t i = 0; i < playerCount && i < 2; ++i) {
        int id = 0;
        string name;
        int row = 0;
        int col = 0;
        int wallsLeft = 0;
        int goalRow = 0;
        ss >> id >> quoted(name) >> row >> col >> wallsLeft >> goalRow;
        Player player(id, name, Position(row, col), goalRow, wallsLeft);
        state.setPlayer(static_cast<int>(i), player);
    }

    size_t wallCount = 0;
    ss >> wallCount;
    for (size_t i = 0; i < wallCount; ++i) {
        int row = 0;
        int col = 0;
        int orientation = 0;
        ss >> row >> col >> orientation;
        state.getBoard().placeWall(Wall(row, col, static_cast<Orientation>(orientation)));
    }

    state.setCurrentTurn(currentTurn);
    state.setStatus(static_cast<GameStatus>(status));
    // history gets reconstructed from persisted undo stack.
    state.clearHistory();

    SaveData dataOut;
    dataOut.state = state;

    if (header == "QDR2") {
        size_t undoCount = 0;
        size_t redoCount = 0;
        ss >> undoCount;
        for (size_t i = 0; i < undoCount; ++i) {
            Move move;
            if (readMove(ss, move)) {
                dataOut.undoStack.push_back(move);
            }
        }

        ss >> redoCount;
        for (size_t i = 0; i < redoCount; ++i) {
            Move move;
            if (readMove(ss, move)) {
                dataOut.redoStack.push_back(move);
            }
        }
    }

    dataOut.state.setHistory(dataOut.undoStack);
    return dataOut;
}
