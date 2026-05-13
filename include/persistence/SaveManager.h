#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include "GameState.h"
#include "Move.h"

using namespace std;

class SaveManager {
public:
    struct SaveData {
        GameState state;
        vector<Move> undoStack;
        vector<Move> redoStack;
    };

    bool save(const GameState& state,
              const vector<Move>& undoStack,
              const vector<Move>& redoStack,
              const string& file);
    SaveData load(const string& file);

private:
    string serialize(const GameState& state,
                     const vector<Move>& undoStack,
                     const vector<Move>& redoStack);
    SaveData deserialize(const string& data);
};

#endif
