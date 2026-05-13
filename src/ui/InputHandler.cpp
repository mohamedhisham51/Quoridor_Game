#include "InputHandler.h"

InputHandler::InputHandler()
    : pendingMove(), pending(false), wallPreviewPos(-1, -1), previewOrientation(Orientation::HORIZONTAL), wallMode(false) {}

void InputHandler::reset() {
    pending = false;
    pendingMove = Move();
    wallPreviewPos = Position(-1, -1);
    wallMode = false;
    previewOrientation = Orientation::HORIZONTAL;
}

Move InputHandler::getPendingMove() const {
    return pendingMove;
}

bool InputHandler::hasPendingMove() const {
    return pending;
}

void InputHandler::handleInput(const GameState& state, const BoardRenderer& renderer) {
    // reset per-frame pending flag; set only on accepted click.
    pending = false;

    if (IsKeyPressed(KEY_W)) {
        wallMode = !wallMode;
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        previewOrientation = (previewOrientation == Orientation::HORIZONTAL) ? Orientation::VERTICAL : Orientation::HORIZONTAL;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        wallPreviewPos = Position(-1, -1);
    }

    const int boardSize = state.getBoard().getBoardSize();
    Vector2 mouse = GetMousePosition();

    if (wallMode) {
        // keep a live preview anchor under the cursor.
        wallPreviewPos = renderer.wallFromMouse(static_cast<int>(mouse.x), static_cast<int>(mouse.y), boardSize);
    }

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return;
    }

    const Player& current = state.getCurrentPlayer();
    if (wallMode) {
        if (!wallPreviewPos.isValid(boardSize - 1)) {
            return;
        }
        pendingMove = Move(current.getId(), Wall(wallPreviewPos.row, wallPreviewPos.col, previewOrientation));
        pending = true;
        return;
    }

    Position clicked = renderer.posFromMouse(static_cast<int>(mouse.x), static_cast<int>(mouse.y), boardSize);
    if (!clicked.isValid(boardSize)) {
        return;
    }

    pendingMove = Move(current.getId(), clicked);
    pending = true;
}

void InputHandler::drawPreview(const GameState& state, const BoardRenderer& renderer) const {
    if (!wallMode || !wallPreviewPos.isValid(state.getBoard().getBoardSize() - 1)) {
        return;
    }

    Rectangle wallRect = renderer.getWallRect(wallPreviewPos.row, wallPreviewPos.col, previewOrientation);
    DrawRectangleRec(wallRect, Color{60, 60, 60, 220});
}
