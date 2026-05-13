#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int row;
    int col;

    Position(int r = 0, int c = 0);

    bool operator==(const Position& other) const;

    // validate that the position is inside board bounds.
    bool isValid(int size) const;
};

#endif