#include "GameScreen.h"

#include <raylib.h>

GameScreen::GameScreen()
    : boardRenderer(),
      hudRenderer(),
      inputHandler(),
      undoButton(Rectangle{80.0f, 60.0f, 84.0f, 34.0f}, "Undo"),
      redoButton(Rectangle{172.0f, 60.0f, 84.0f, 34.0f}, "Redo"),
      saveButton(Rectangle{264.0f, 60.0f, 84.0f, 34.0f}, "Save"),
      loadButton(Rectangle{356.0f, 60.0f, 84.0f, 34.0f}, "Load"),
      resetButton(Rectangle{448.0f, 60.0f, 84.0f, 34.0f}, "Reset"),
      menuButton(Rectangle{540.0f, 60.0f, 140.0f, 34.0f}, "Main Menu"),
      active(true),
      returnToMenu(false),
      statusText("") {}

void GameScreen::updateLayout(const GameState& state) {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    float topHeight = std::max(60.0f, screenH * 0.12f);
    float bottomHeight = std::max(170.0f, screenH * 0.22f);

    int buttonCount = 6;
    float gap = std::max(8.0f, screenW * 0.015f);
    float buttonHeight = std::max(32.0f, topHeight * 0.55f);
    float buttonWidth = std::min(150.0f, screenW * 0.13f);

    float maxWidth = screenW * 0.92f;
    float totalWidth = buttonCount * buttonWidth + (buttonCount - 1) * gap;
    if (totalWidth > maxWidth) {
        buttonWidth = (maxWidth - (buttonCount - 1) * gap) / buttonCount;
        if (buttonWidth < 70.0f) {
            gap = std::max(6.0f, gap * 0.7f);
            buttonWidth = (maxWidth - (buttonCount - 1) * gap) / buttonCount;
        }
        totalWidth = buttonCount * buttonWidth + (buttonCount - 1) * gap;
    }

    float startX = (screenW - totalWidth) * 0.5f;
    float startY = (topHeight - buttonHeight) * 0.5f;

    undoButton.bounds = Rectangle{startX + 0 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};
    redoButton.bounds = Rectangle{startX + 1 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};
    saveButton.bounds = Rectangle{startX + 2 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};
    loadButton.bounds = Rectangle{startX + 3 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};
    resetButton.bounds = Rectangle{startX + 4 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};
    menuButton.bounds = Rectangle{startX + 5 * (buttonWidth + gap), startY, buttonWidth, buttonHeight};

    boardRenderer.updateLayout(screenW, screenH, topHeight, bottomHeight, state.getBoard().getBoardSize());
    hudRenderer.updateLayout(screenW, screenH, boardRenderer.getBoardRect());
}

bool GameScreen::isActive() const {
    return active;
}

bool GameScreen::shouldReturnToMenu() const {
    return returnToMenu;
}

void GameScreen::clearReturnToMenu() {
    returnToMenu = false;
}

Move GameScreen::getMoveInput() const {
    return inputHandler.getPendingMove();
}

void GameScreen::update(GameController& controller) {
    const GameState& state = controller.getState();

    // recompute responsive layout each frame.
    updateLayout(state);

    undoButton.update();
    redoButton.update();
    saveButton.update();
    loadButton.update();
    resetButton.update();
    menuButton.update();

    if (undoButton.isClicked()) {
        if (!controller.undo()) {
            hudRenderer.drawMessage("Nothing to undo");
        }
    }
    if (redoButton.isClicked()) {
        if (!controller.redo()) {
            hudRenderer.drawMessage("Nothing to redo");
        }
    }
    if (saveButton.isClicked()) {
        if (!controller.saveGame("savegame.qdr")) {
            hudRenderer.drawMessage("Save failed");
        } else {
            hudRenderer.drawMessage("Saved to savegame.qdr");
        }
    }
    if (loadButton.isClicked()) {
        if (!controller.loadGame("savegame.qdr")) {
            hudRenderer.drawMessage("Load failed");
        } else {
            hudRenderer.drawMessage("Loaded savegame.qdr");
        }
    }
    if (resetButton.isClicked()) {
        controller.resetGame();
        inputHandler.reset();
        hudRenderer.drawMessage("Game reset");
    }
    if (menuButton.isClicked()) {
        returnToMenu = true;
    }

    if (state.isGameOver()) {
        return;
    }

    // highlight legal pawn destinations for current player.
    std::vector<Move> valid = state.getValidMovesForPlayer(state.getCurrentPlayer().getId());
    boardRenderer.highlightMoves(valid, state.getCurrentPlayer().getId());

    inputHandler.handleInput(state, boardRenderer);
    if (inputHandler.hasPendingMove()) {
        if (!controller.processMove(inputHandler.getPendingMove())) {
            hudRenderer.drawMessage("Illegal move");
        }
    }
}

void GameScreen::draw(const GameController& controller) {
    const GameState& state = controller.getState();

    updateLayout(state);

    undoButton.draw();
    redoButton.draw();
    saveButton.draw();
    loadButton.draw();
    resetButton.draw();
    menuButton.draw();

    boardRenderer.draw(state);
    inputHandler.drawPreview(state, boardRenderer);
    hudRenderer.draw(state);
}
