#include "HudRenderer.h"

#include <algorithm>

static void drawBoldText(const char* text, int x, int y, int fontSize, Color color) {
    DrawText(text, x, y, fontSize, color);
    DrawText(text, x + 1, y, fontSize, color);
    DrawText(text, x, y + 1, fontSize, color);
}

HudRenderer::HudRenderer()
    : message(""),
      messageUntil(0.0f),
      screenWidth(900.0f),
      screenHeight(820.0f),
      boardRect(Rectangle{80.0f, 120.0f, 540.0f, 540.0f}) {}

void HudRenderer::updateLayout(int screenWidthValue, int screenHeightValue, const Rectangle& boardRectValue) {
    screenWidth = static_cast<float>(screenWidthValue);
    screenHeight = static_cast<float>(screenHeightValue);
    boardRect = boardRectValue;
}

void HudRenderer::drawMessage(const std::string& text) const {
    message = text;
    messageUntil = GetTime() + 1.8f;
}

void HudRenderer::draw(const GameState& state) const {
    const std::vector<Player>& players = state.getPlayers();
    // anchor hud just below the board rect.
    float padding = std::max(8.0f, screenHeight * 0.012f);
    float baseX = boardRect.x;
    float baseY = boardRect.y + boardRect.height + padding;

    int turnFont = std::max(26, static_cast<int>(screenHeight * 0.032f));
    int wallFont = std::max(22, static_cast<int>(screenHeight * 0.027f));
    int detailFont = std::max(20, static_cast<int>(screenHeight * 0.024f));
    int instructionFont = std::max(20, static_cast<int>(screenHeight * 0.024f));
    int alertFont = std::max(26, static_cast<int>(screenHeight * 0.032f));
    float lineGap = std::max(6.0f, screenHeight * 0.008f);

    bool isAiMode = state.getConfig().mode == GameMode::HUMAN_VS_AI;

    if (!players.empty()) {
        const Player& current = state.getCurrentPlayer();
        Color pawnColors[2] = {Color{52, 152, 219, 255}, Color{231, 76, 60, 255}};
        int playerIndex = (current.getId() == 2) ? 1 : 0;
        float circleRadius = turnFont * 0.35f;
        float circleX = baseX + circleRadius;
        float circleY = baseY + (turnFont * 0.5f);
        DrawCircle(static_cast<int>(circleX), static_cast<int>(circleY), circleRadius, pawnColors[playerIndex]);
        DrawCircleLines(static_cast<int>(circleX), static_cast<int>(circleY), circleRadius, BLACK);

        float textX = baseX + (circleRadius * 2.0f) + 8.0f;
        const char* turnText = (isAiMode && current.getId() == 2) ? "AI's Turn" : "Player 1's Turn";
        if (!isAiMode && current.getId() == 2) {
            turnText = "Player 2's Turn";
        }
        drawBoldText(turnText, static_cast<int>(textX), static_cast<int>(baseY), turnFont, Color{36, 54, 94, 255});
    }

    float wallY = baseY + turnFont + lineGap;
    if (players.size() > 0) {
        std::string p1Text = TextFormat("Player 1 walls: %d", players[0].getWallsLeft());
        drawBoldText(p1Text.c_str(), static_cast<int>(baseX), static_cast<int>(wallY), wallFont, DARKBROWN);
    }
    if (players.size() > 1) {
        const char* label = (isAiMode) ? "AI" : "Player 2";
        std::string p2Text = TextFormat("%s walls: %d", label, players[1].getWallsLeft());
        int textWidth = MeasureText(p2Text.c_str(), wallFont);
        float rightX = boardRect.x + boardRect.width - static_cast<float>(textWidth);
        drawBoldText(p2Text.c_str(), static_cast<int>(rightX), static_cast<int>(wallY), wallFont, DARKBROWN);
    }

    std::string modeText = isAiMode ? "Human vs AI" : "Human vs Human";
    std::string details = TextFormat("Mode: %s | Size: %dx%d",
                                     modeText.c_str(),
                                     state.getBoard().getBoardSize(),
                                     state.getBoard().getBoardSize());
    if (isAiMode) {
        std::string diffText = "Easy";
        if (state.getConfig().difficulty == Difficulty::MEDIUM) {
            diffText = "Medium";
        } else if (state.getConfig().difficulty == Difficulty::HARD) {
            diffText = "Hard";
        }
        details = TextFormat("Mode: %s | Size: %dx%d | Difficulty: %s",
                             modeText.c_str(),
                             state.getBoard().getBoardSize(),
                             state.getBoard().getBoardSize(),
                             diffText.c_str());
    }
    float detailsY = wallY + wallFont + lineGap;
    drawBoldText(details.c_str(), static_cast<int>(baseX), static_cast<int>(detailsY), detailFont, Color{70, 70, 70, 255});

    float instructionY = detailsY + detailFont + lineGap;
    drawBoldText("W: Wall mode, Right-click: Rotate", static_cast<int>(baseX), static_cast<int>(instructionY),
                 instructionFont, Color{70, 70, 70, 255});

    std::string alertText;
    Color alertColor = Color{170, 45, 45, 255};
    if (!message.empty() && GetTime() < messageUntil) {
        alertText = message;
    } else if (state.getStatus() == GameStatus::PLAYER1_WIN) {
        alertText = "Player 1 wins!";
        alertColor = Color{28, 158, 98, 255};
    } else if (state.getStatus() == GameStatus::PLAYER2_WIN) {
        alertText = isAiMode ? "AI wins!" : "Player 2 wins!";
        alertColor = Color{28, 158, 98, 255};
    }

    if (!alertText.empty()) {
        float desiredY = instructionY + instructionFont + lineGap * 0.6f;
        float bottomCap = screenHeight - padding - alertFont;
        float alertY = std::min(desiredY, bottomCap);
        drawBoldText(alertText.c_str(), static_cast<int>(baseX), static_cast<int>(alertY),
                     alertFont, alertColor);
    }
}
