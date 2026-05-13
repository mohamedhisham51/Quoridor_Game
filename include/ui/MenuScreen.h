#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <raylib.h>
#include "Button.h"
#include "GameConfig.h"

class MenuScreen {
public:
    MenuScreen();

    void draw();
    void update();
    GameConfig getConfig() const;
    bool isDone() const;
    void reset();

private:
    void updateLayout();
    bool done;
    int boardSize;
    bool boardEdit;
    int selectedMode;
    int selectedDiff;
    Button startButton;
};

#endif
