#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <raylib.h>

class Button {
public:
    Rectangle bounds;
    std::string label;

    Button();
    Button(Rectangle rect, const std::string& text);

    void draw() const;
    void update();
    bool isClicked() const;

private:
    bool hovered;
};

#endif
