#include "GameController.h"

GameController::GameController()
    : state(GameConfig()), saveManager(nullptr) {
    players[0] = nullptr;
    players[1] = nullptr;
}

GameController::GameController(const GameConfig& config)
    : state(config), saveManager(nullptr) {
    players[0] = nullptr;
    players[1] = nullptr;
}

void GameController::setPlayers(IPlayer* p1, IPlayer* p2) {
    players[0] = p1;
    players[1] = p2;
}

void GameController::setSaveManager(SaveManager* manager) {
    saveManager = manager;
}

void GameController::startGame() {
    // reset state and history stacks for a fresh session.
    state.reset();
    undoStack.clear();
    redoStack.clear();
}

bool GameController::processMove(const Move& move) {
    if (!state.applyMove(move)) {
        return false;
    }

    // successful new move invalidates any redo branch.
    undoStack.push_back(move);
    redoStack.clear();
    return true;
}

bool GameController::undo() {
    if (undoStack.empty()) {
        return false;
    }

    int undoCount = 1;
    if (state.getConfig().mode == GameMode::HUMAN_VS_AI) {
        // undo both half-turns so human returns to pre-ai state.
        if (undoStack.size() < 2) {
            return false;
        }
        undoCount = 2;
    }

    for (int i = 0; i < undoCount; ++i) {
        Move lastMove = undoStack.back();
        undoStack.pop_back();
        redoStack.push_back(lastMove);
        if (!state.undoMove()) {
            return false;
        }
    }

    return true;
}

bool GameController::redo() {
    if (redoStack.empty()) {
        return false;
    }

    if (state.getConfig().mode == GameMode::HUMAN_VS_AI) {
        // redo is limited to one branch in ai mode.
        Move move = redoStack.back();
        redoStack.pop_back();
        if (!state.applyMove(move)) {
            return false;
        }
        undoStack.push_back(move);
        // clear remaining redo entries to avoid desync with ai turns.
        redoStack.clear();
        return true;
    }

    Move move = redoStack.back();
    redoStack.pop_back();
    if (!state.applyMove(move)) {
        return false;
    }
    undoStack.push_back(move);
    return true;
}

bool GameController::saveGame(const string& filename) {
    if (!saveManager) {
        return false;
    }
    return saveManager->save(state, undoStack, redoStack, filename);
}

bool GameController::loadGame(const string& filename) {
    if (!saveManager) {
        return false;
    }

    SaveManager::SaveData data = saveManager->load(filename);
    // replace runtime state and both history stacks from save data.
    state = data.state;
    undoStack = data.undoStack;
    redoStack = data.redoStack;
    return true;
}

void GameController::resetGame() {
    state.reset();
    undoStack.clear();
    redoStack.clear();
}

const GameState& GameController::getState() const {
    return state;
}
