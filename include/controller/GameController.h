#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>
#include <string>
#include "GameState.h"
#include "IPlayer.h"
#include "SaveManager.h"

using namespace std;

class GameController {
private:
    GameState state;
    IPlayer* players[2];
    vector<Move> undoStack;
    vector<Move> redoStack;
    SaveManager* saveManager;

public:
    GameController();
    explicit GameController(const GameConfig& config);

    void setPlayers(IPlayer* p1, IPlayer* p2);
    void setSaveManager(SaveManager* manager);

    void startGame();
    bool processMove(const Move& move);
    bool undo();
    bool redo();
    bool saveGame(const string& filename);
    bool loadGame(const string& filename);
    void resetGame();

    const GameState& getState() const;
};

#endif
