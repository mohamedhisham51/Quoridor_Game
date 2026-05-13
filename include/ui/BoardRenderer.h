#ifndef BOARDRENDERER_H
#define BOARDRENDERER_H

#include <vector>
#include <raylib.h>
#include "GameState.h"

class BoardRenderer {
public:
    BoardRenderer();

    void updateLayout(int screenWidth, int screenHeight, float topHeight, float bottomHeight, int boardSize);
    void draw(const GameState& state) const;
    void drawWalls(const GameState& state) const;
    void highlightMoves(const std::vector<Move>& moves, int playerId);
    Position posFromMouse(int x, int y, int boardSize) const;

    Position wallFromMouse(int x, int y, int boardSize) const;
    float getCellSize() const;
    float getGrooveSize() const;
    float getOffsetX() const;
    float getOffsetY() const;
    Rectangle getBoardRect() const;
    Rectangle getWallRect(int row, int col, Orientation orientation) const;
    bool isInsideBoard(int x, int y, int boardSize) const;

private:
    float cellSize;
    float offsetX;
    float offsetY;
    float groove;
    float boardSizePx;
    Color highlightColor;
    std::vector<Position> highlighted;
};

#endif
