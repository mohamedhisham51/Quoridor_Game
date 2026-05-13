#include "BoardRenderer.h"

#include <algorithm>
#include <cmath>

BoardRenderer::BoardRenderer()
    : cellSize(60.0f),
      offsetX(80.0f),
      offsetY(100.0f),
      groove(6.0f),
      boardSizePx(0.0f),
      highlightColor(Color{170, 225, 170, 255}) {}

void BoardRenderer::updateLayout(int screenWidth, int screenHeight, float topHeight, float bottomHeight, int boardSize) {
    float availableHeight = static_cast<float>(screenHeight) - topHeight - bottomHeight;
    float availableWidth = static_cast<float>(screenWidth) * 0.92f;
    // fit a square board inside the free area.
    float targetSize = std::min(availableWidth, availableHeight);
    if (targetSize < 50.0f) {
        targetSize = 50.0f;
    }

    groove = std::max(3.0f, targetSize * 0.015f);
    float rawCell = (targetSize - (boardSize + 1) * groove) / static_cast<float>(boardSize);
    if (rawCell < 10.0f) {
        rawCell = 10.0f;
    }
    cellSize = floorf(rawCell);
    boardSizePx = (boardSize * cellSize) + (boardSize + 1) * groove;

    offsetX = (static_cast<float>(screenWidth) - boardSizePx) * 0.5f;
    if (offsetX < 0.0f) {
        offsetX = 0.0f;
    }

    float boardTop = topHeight + (availableHeight - boardSizePx) * 0.5f;
    if (boardTop < topHeight) {
        boardTop = topHeight;
    }
    offsetY = boardTop;
}

void BoardRenderer::highlightMoves(const std::vector<Move>& moves, int playerId) {
    highlighted.clear();
    highlightColor = (playerId == 2)
        ? Color{255, 160, 160, 255}
        : Color{130, 195, 255, 255};
    for (const Move& move : moves) {
        if (!move.isWallMove()) {
            highlighted.push_back(move.target);
        }
    }
}

bool BoardRenderer::isInsideBoard(int x, int y, int boardSize) const {
    float sizePx = boardSizePx;
    return x >= offsetX && x < (offsetX + sizePx) && y >= offsetY && y < (offsetY + sizePx);
}

Position BoardRenderer::posFromMouse(int x, int y, int boardSize) const {
    if (!isInsideBoard(x, y, boardSize)) {
        return Position(-1, -1);
    }

    float relX = static_cast<float>(x) - offsetX - groove;
    float relY = static_cast<float>(y) - offsetY - groove;
    float cellPitch = cellSize + groove;

    int col = static_cast<int>(floorf(relX / cellPitch));
    int row = static_cast<int>(floorf(relY / cellPitch));

    if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
        return Position(-1, -1);
    }

    float localX = relX - col * cellPitch;
    float localY = relY - row * cellPitch;
    if (localX < 0.0f || localY < 0.0f || localX > cellSize || localY > cellSize) {
        return Position(-1, -1);
    }

    return Position(row, col);
}

Position BoardRenderer::wallFromMouse(int x, int y, int boardSize) const {
    float relX = static_cast<float>(x) - offsetX - groove;
    float relY = static_cast<float>(y) - offsetY - groove;
    float cellPitch = cellSize + groove;

    int col = static_cast<int>(floorf(relX / cellPitch));
    int row = static_cast<int>(floorf(relY / cellPitch));
    if (row < 0 || row >= boardSize - 1 || col < 0 || col >= boardSize - 1) {
        return Position(-1, -1);
    }
    return Position(row, col);
}

float BoardRenderer::getCellSize() const {
    return cellSize;
}

float BoardRenderer::getGrooveSize() const {
    return groove;
}

float BoardRenderer::getOffsetX() const {
    return offsetX;
}

float BoardRenderer::getOffsetY() const {
    return offsetY;
}

Rectangle BoardRenderer::getBoardRect() const {
    return Rectangle{offsetX, offsetY, boardSizePx, boardSizePx};
}

Rectangle BoardRenderer::getWallRect(int row, int col, Orientation orientation) const {
    float cellPitch = cellSize + groove;
    float cellX = offsetX + groove + col * cellPitch;
    float cellY = offsetY + groove + row * cellPitch;

    float wallThickness = std::max(4.0f, groove * 0.75f);
    float wallInset = std::max(2.0f, groove * 0.55f);
    float wallLength = (2.0f * cellSize) + groove - (2.0f * wallInset);

    if (orientation == Orientation::HORIZONTAL) {
        float x = cellX + wallInset;
        float y = cellY + cellSize + (groove - wallThickness) * 0.5f;
        return Rectangle{x, y, wallLength, wallThickness};
    }

    float x = cellX + cellSize + (groove - wallThickness) * 0.5f;
    float y = cellY + wallInset;
    return Rectangle{x, y, wallThickness, wallLength};
}

void BoardRenderer::drawWalls(const GameState& state) const {
    const std::vector<Wall>& walls = state.getBoard().getWalls();

    for (const Wall& wall : walls) {
        Rectangle wallRect = getWallRect(wall.row, wall.col, wall.orientation);
        DrawRectangleRec(wallRect, Color{110, 60, 30, 255});
    }
}

void BoardRenderer::draw(const GameState& state) const {
    int boardSize = state.getBoard().getBoardSize();

    DrawRectangle(static_cast<int>(offsetX), static_cast<int>(offsetY),
                  static_cast<int>(boardSizePx), static_cast<int>(boardSizePx),
                  Color{170, 140, 100, 255});

    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            // precompute repeated spacing math for this cell.
            float cellPitch = cellSize + groove;
            int x = static_cast<int>(offsetX + groove + col * cellPitch);
            int y = static_cast<int>(offsetY + groove + row * cellPitch);
            Color fill = ((row + col) % 2 == 0) ? Color{240, 215, 170, 255} : Color{228, 198, 150, 255};

            bool isHighlighted = std::any_of(highlighted.begin(), highlighted.end(), [row, col](const Position& p) {
                return p.row == row && p.col == col;
            });

            if (isHighlighted) {
                fill = highlightColor;
            }

            DrawRectangle(x, y, static_cast<int>(cellSize), static_cast<int>(cellSize), fill);
            DrawRectangleLines(x, y, static_cast<int>(cellSize), static_cast<int>(cellSize), Color{80, 55, 30, 255});
        }
    }

    drawWalls(state);

    const std::vector<Player>& players = state.getPlayers();
    Color pawnColors[2] = {Color{52, 152, 219, 255}, Color{231, 76, 60, 255}};

    for (int i = 0; i < static_cast<int>(players.size()) && i < 2; ++i) {
        Position pos = players[i].getPosition();
        float cellPitch = cellSize + groove;
        int cx = static_cast<int>(offsetX + groove + (pos.col * cellPitch) + (cellSize * 0.5f));
        int cy = static_cast<int>(offsetY + groove + (pos.row * cellPitch) + (cellSize * 0.5f));
        float radius = cellSize * 0.33f;
        DrawCircle(cx, cy, radius, pawnColors[i]);
        DrawCircleLines(cx, cy, radius, BLACK);
    }
}
