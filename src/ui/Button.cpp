#include "Button.h"

Button::Button() : bounds{0.0f, 0.0f, 0.0f, 0.0f}, label(""), hovered(false) {}

Button::Button(Rectangle rect, const std::string& text)
    : bounds(rect), label(text), hovered(false) {}

void Button::update() {
    hovered = CheckCollisionPointRec(GetMousePosition(), bounds);
}

bool Button::isClicked() const {
    return hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

void Button::draw() const {
    Color fill = hovered ? Color{70, 110, 170, 255} : Color{45, 70, 110, 255};
    DrawRectangleRec(bounds, fill);
    DrawRectangleLinesEx(bounds, 2.0f, Color{200, 220, 245, 255});

    int fontSize = 20;
    // center text label inside the button rect.
    int textWidth = MeasureText(label.c_str(), fontSize);
    int textX = static_cast<int>(bounds.x + (bounds.width - static_cast<float>(textWidth)) * 0.5f);
    int textY = static_cast<int>(bounds.y + (bounds.height - static_cast<float>(fontSize)) * 0.5f);
    DrawText(label.c_str(), textX, textY, fontSize, RAYWHITE);
}
