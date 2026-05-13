#ifndef CELL_H
#define CELL_H

class Cell {
public:
    int row;
    int col;
    
    // wall flags mark blocked exits for each direction.
    bool hasWallN; // north (up)
    bool hasWallE; // east (right)
    bool hasWallS; // south (down)
    bool hasWallW; // west (left)

    Cell(int r = 0, int c = 0);
    
    // clear wall flags so the cell can be reused on reset.
    void clearWalls();
};

#endif