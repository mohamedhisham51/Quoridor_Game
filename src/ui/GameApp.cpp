#include "GameApp.h"

#include <chrono>
#include <raylib.h>
#include <future>

void GameApp::resetAiMoveState() {
    if (aiMoveFuture.valid()) {
        aiMoveFuture = std::future<Move>();
    }
    aiThinking = false;
}

GameApp::GameApp() : aiStrategy(nullptr), aiThinking(false), currentScreen(MENU_SCREEN) {
    menuScreen.reset(new MenuScreen());
    gameScreen.reset(new GameScreen());
    saveManager.reset(new SaveManager());
}

GameApp::~GameApp() {
    shutdown();
}

void GameApp::init(const GameConfig& config) {
    resetAiMoveState();

    controller.reset(new GameController(config));
    controller->setSaveManager(saveManager.get());

    humanPlayer1.reset(new HumanPlayer());

    if (config.mode == GameMode::HUMAN_VS_HUMAN) {
        humanPlayer2.reset(new HumanPlayer());
        aiPlayer.reset();
        controller->setPlayers(humanPlayer1.get(), humanPlayer2.get());
    } else {
        humanPlayer2.reset();

        // create only the strategy selected by menu difficulty.
        if (config.difficulty == Difficulty::EASY) {
            aiStrategy.reset(new RandomStrategy());
        } else if (config.difficulty == Difficulty::MEDIUM) {
            aiStrategy.reset(new GreedyStrategy());
        } else {
            aiStrategy.reset(new MinimaxStrategy(2));
        }

        aiPlayer.reset(new AIPlayer(2, aiStrategy.get()));
        controller->setPlayers(humanPlayer1.get(), aiPlayer.get());
    }

    controller->startGame();
}

void GameApp::update() {
    if (currentScreen == MENU_SCREEN) {
        menuScreen->update();
        if (menuScreen->isDone()) {
            init(menuScreen->getConfig());
            gameScreen->clearReturnToMenu();
            currentScreen = GAME_SCREEN;
        }
        return;
    }

    if (!controller) {
        currentScreen = MENU_SCREEN;
        menuScreen->reset();
        resetAiMoveState();
        return;
    }

    const GameState& state = controller->getState();
    const int currentPlayerId = state.getCurrentPlayer().getId();

    if (aiPlayer && currentPlayerId == 2 && !state.isGameOver()) {
        if (!aiThinking && !aiMoveFuture.valid()) {
            aiThinking = true;
            GameState stateCopy = state;
            IAIStrategy* strategy = aiStrategy.get();
            aiMoveFuture = std::async(std::launch::async, [strategy, stateCopy]() mutable {
                if (!strategy) {
                    return Move();
                }
                // run ai search on a worker thread to keep frame updates responsive.
                return strategy->selectMove(stateCopy, 2);
            });
            return;
        }

        if (aiMoveFuture.valid()) {
            if (aiMoveFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                aiThinking = true;
                return;
            }

            Move aiMove = aiMoveFuture.get();
            aiMoveFuture = std::future<Move>();
            aiThinking = false;
            controller->processMove(aiMove);
        }
    } else {
        resetAiMoveState();
        gameScreen->update(*controller);
    }

    if (gameScreen->shouldReturnToMenu()) {
        currentScreen = MENU_SCREEN;
        menuScreen->reset();
        gameScreen->clearReturnToMenu();
    }
}

void GameApp::draw() {
    if (currentScreen == MENU_SCREEN) {
        menuScreen->draw();
    } else if (controller) {
        gameScreen->draw(*controller);
        
        // draw a small overlay while the async ai move is still pending.
        if (aiThinking) {
            const char* thinkingText = "AI is thinking...";
            int fontSize = 28;
            int textWidth = MeasureText(thinkingText, fontSize);
            int screenWidth = GetScreenWidth();
            int screenHeight = GetScreenHeight();
            int x = (screenWidth - textWidth) / 2;
            int y = screenHeight / 2 - fontSize / 2;
            DrawRectangle(x - 20, y - 15, textWidth + 40, fontSize + 30, Color{200, 200, 200, 220});
            DrawRectangleLines(x - 20, y - 15, textWidth + 40, fontSize + 30, Color{100, 100, 100, 255});
            DrawText(thinkingText, x, y, fontSize, Color{36, 54, 94, 255});
        }
    }
}

void GameApp::run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(900, 820, "Quoridor Game");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        update();

        BeginDrawing();
        ClearBackground(Color{247, 245, 239, 255});
        draw();
        EndDrawing();
    }

    CloseWindow();
}

void GameApp::shutdown() {
    resetAiMoveState();
    controller.reset();
    humanPlayer1.reset();
    humanPlayer2.reset();
    aiPlayer.reset();
    aiStrategy.reset();
}
