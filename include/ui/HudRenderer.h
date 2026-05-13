#ifndef HUDRENDERER_H
#define HUDRENDERER_H

#include <string>
#include <raylib.h>
#include "GameState.h"

class HudRenderer {
public:
    HudRenderer();

    void updateLayout(int screenWidth, int screenHeight, const Rectangle& boardRect);
    void draw(const GameState& state) const;
    void drawMessage(const std::string& message) const;

private:
    mutable std::string message;
    mutable float messageUntil;
    float screenWidth;
    float screenHeight;
    Rectangle boardRect;
};

#endif
