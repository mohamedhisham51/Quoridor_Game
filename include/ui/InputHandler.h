#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <raylib.h>
#include "BoardRenderer.h"
#include "GameState.h"

class InputHandler {
public:
    InputHandler();

    void handleInput(const GameState& state, const BoardRenderer& renderer);
    Move getPendingMove() const;
    bool hasPendingMove() const;
    void reset();
    void drawPreview(const GameState& state, const BoardRenderer& renderer) const;

private:
    Move pendingMove;
    bool pending;
    Position wallPreviewPos;
    Orientation previewOrientation;
    bool wallMode;
};

#endif
