#ifndef ENUMS_H
#define ENUMS_H

enum class Orientation {
    HORIZONTAL,
    VERTICAL
};

enum class MoveType {
    PAWN_MOVE,
    WALL_PLACE
};

enum class GameStatus {
    IN_PROGRESS,
    PLAYER1_WIN,
    PLAYER2_WIN
};

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

enum class GameMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI
};

#endif