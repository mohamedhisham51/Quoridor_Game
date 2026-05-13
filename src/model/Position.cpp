#include "Position.h"

// store row and column grid coordinates.
Position::Position(int r, int c) : row(r), col(c) {}

// positions are equal when both row and column match.
bool Position::operator==(const Position& other) const {
    return (this->row == other.row) && (this->col == other.col);
}

// keep coordinates inside [0, size).
bool Position::isValid(int size) const {
    return (row >= 0 && row < size && col >= 0 && col < size);
}