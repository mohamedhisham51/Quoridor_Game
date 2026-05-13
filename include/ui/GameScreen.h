#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <string>
#include "BoardRenderer.h"
#include "Button.h"
#include "GameController.h"
#include "HudRenderer.h"
#include "InputHandler.h"

class GameScreen {
public:
    GameScreen();

    void update(GameController& controller);
    void draw(const GameController& controller);
    Move getMoveInput() const;
    bool isActive() const;

    bool shouldReturnToMenu() const;
    void clearReturnToMenu();

private:
    void updateLayout(const GameState& state);
    BoardRenderer boardRenderer;
    HudRenderer hudRenderer;
    InputHandler inputHandler;

    Button undoButton;
    Button redoButton;
    Button saveButton;
    Button loadButton;
    Button resetButton;
    Button menuButton;

    bool active;
    bool returnToMenu;
    std::string statusText;
};

#endif
