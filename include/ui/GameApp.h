#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <future>
#include <memory>
#include "AIPlayer.h"
#include "GameController.h"
#include "GameScreen.h"
#include "GreedyStrategy.h"
#include "HumanPlayer.h"
#include "MenuScreen.h"
#include "MinimaxStrategy.h"
#include "RandomStrategy.h"
#include "SaveManager.h"

class GameApp {
public:
    GameApp();
    ~GameApp();

    void run();
    void init(const GameConfig& config);
    void update();
    void draw();
    void shutdown();

private:
    enum ScreenId {
        MENU_SCREEN = 0,
        GAME_SCREEN = 1
    };

    std::unique_ptr<GameController> controller;
    std::unique_ptr<GameScreen> gameScreen;
    std::unique_ptr<MenuScreen> menuScreen;
    std::unique_ptr<SaveManager> saveManager;

    std::unique_ptr<HumanPlayer> humanPlayer1;
    std::unique_ptr<HumanPlayer> humanPlayer2;
    std::unique_ptr<AIPlayer> aiPlayer;

    std::unique_ptr<IAIStrategy> aiStrategy;
    std::future<Move> aiMoveFuture;
    bool aiThinking;
    int currentScreen;

    void resetAiMoveState();
};

#endif
