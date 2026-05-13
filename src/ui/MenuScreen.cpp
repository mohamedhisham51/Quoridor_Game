#include "MenuScreen.h"

#include <algorithm>

#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"

MenuScreen::MenuScreen()
    : done(false),
      boardSize(9),
      boardEdit(false),
      selectedMode(0),
      selectedDiff(0),
      startButton(Rectangle{310.0f, 480.0f, 220.0f, 56.0f}, "Start Game") {}

void MenuScreen::updateLayout() {
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    float panelW = std::min(560.0f, screenW * 0.72f);
    float panelH = std::min(360.0f, screenH * 0.46f);
    float panelX = (screenW - panelW) * 0.5f;
    float panelY = (screenH - panelH) * 0.5f + screenH * 0.06f;

    float buttonW = std::min(240.0f, panelW * 0.55f);
    float buttonH = std::max(44.0f, panelH * 0.16f);
    float buttonX = panelX + (panelW - buttonW) * 0.5f;
    float buttonY = panelY + panelH - buttonH - std::max(16.0f, panelH * 0.08f);

    startButton.bounds = Rectangle{buttonX, buttonY, buttonW, buttonH};
}

void MenuScreen::reset() {
    done = false;
}

bool MenuScreen::isDone() const {
    return done;
}

void MenuScreen::update() {
    updateLayout();
    startButton.update();
    if (startButton.isClicked()) {
        done = true;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        done = true;
    }
}

GameConfig MenuScreen::getConfig() const {
    GameMode mode = (selectedMode == 0) ? GameMode::HUMAN_VS_HUMAN : GameMode::HUMAN_VS_AI;

    // default to easy and map combo index to enum.
    Difficulty diff = Difficulty::EASY;
    if (selectedDiff == 1) {
        diff = Difficulty::MEDIUM;
    } else if (selectedDiff == 2) {
        diff = Difficulty::HARD;
    }

    return GameConfig(boardSize, 10, diff, mode);
}

void MenuScreen::draw() {
    updateLayout();

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    float panelW = std::min(560.0f, screenW * 0.72f);
    float panelH = std::min(360.0f, screenH * 0.46f);
    float panelX = (screenW - panelW) * 0.5f;
    float panelY = (screenH - panelH) * 0.5f + screenH * 0.06f;

    int titleFont = std::max(36, static_cast<int>(screenH * 0.075f));
    int subtitleFont = std::max(18, static_cast<int>(screenH * 0.03f));

    const char* title = "Quoridor";
    int titleWidth = MeasureText(title, titleFont);
    DrawText(title, (screenW - titleWidth) / 2, static_cast<int>(screenH * 0.08f), titleFont, Color{32, 47, 86, 255});

    const char* subtitle = "Cse472s - Artificial Intelligence";
    int subtitleWidth = MeasureText(subtitle, subtitleFont);
    DrawText(subtitle, (screenW - subtitleWidth) / 2, static_cast<int>(screenH * 0.08f + titleFont + 6),
             subtitleFont, Color{78, 95, 127, 255});

    DrawRectangle(static_cast<int>(panelX), static_cast<int>(panelY), static_cast<int>(panelW), static_cast<int>(panelH),
                  Color{244, 239, 228, 255});
    DrawRectangleLines(static_cast<int>(panelX), static_cast<int>(panelY), static_cast<int>(panelW), static_cast<int>(panelH),
                       Color{80, 70, 52, 255});

    float paddingX = std::max(20.0f, panelW * 0.08f);
    float labelX = panelX + paddingX;
    float controlW = panelW * 0.38f;
    float controlX = panelX + panelW - paddingX - controlW;

    float rowStart = panelY + panelH * 0.18f;
    float rowGap = panelH * 0.18f;
    int labelFont = std::max(18, static_cast<int>(panelH * 0.08f));
    float controlH = std::max(30.0f, panelH * 0.12f);

    int controlTextSize = std::max(16, static_cast<int>(controlH * 0.45f));
    int oldTextSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
    int oldComboWidth = GuiGetStyle(COMBOBOX, COMBO_BUTTON_WIDTH);
    // temporarily tune gui style to match responsive control size.
    int comboWidth = std::max(28, static_cast<int>(controlH * 0.9f));
    GuiSetStyle(DEFAULT, TEXT_SIZE, controlTextSize);
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, comboWidth);

    DrawText("Board Size", static_cast<int>(labelX), static_cast<int>(rowStart), labelFont, DARKGRAY);
    if (GuiSpinner(Rectangle{controlX, rowStart - 4.0f, controlW, controlH}, NULL, &boardSize, 5, 12, boardEdit)) {
        boardEdit = !boardEdit;
    }

    float row2 = rowStart + rowGap;
    DrawText("Game Mode", static_cast<int>(labelX), static_cast<int>(row2), labelFont, DARKGRAY);
    GuiComboBox(Rectangle{controlX, row2 - 4.0f, controlW, controlH}, "Human vs Human;Human vs AI", &selectedMode);

    float row3 = row2 + rowGap;
    if (selectedMode == 1) {
        DrawText("AI Difficulty", static_cast<int>(labelX), static_cast<int>(row3), labelFont, DARKGRAY);
        GuiComboBox(Rectangle{controlX, row3 - 4.0f, controlW, controlH}, "Easy;Medium;Hard", &selectedDiff);
    }

    // restore global gui style after drawing controls.
    GuiSetStyle(COMBOBOX, COMBO_BUTTON_WIDTH, oldComboWidth);
    GuiSetStyle(DEFAULT, TEXT_SIZE, oldTextSize);

    int hintFont = std::max(16, static_cast<int>(screenH * 0.025f));
    const char* hint = "Press W in game for wall mode";
    int hintWidth = MeasureText(hint, hintFont);
    DrawText(hint, (screenW - hintWidth) / 2, static_cast<int>(panelY + panelH + 16), hintFont, Color{70, 70, 70, 255});
    startButton.draw();
}
