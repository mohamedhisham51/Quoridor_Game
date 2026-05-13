#include "Wall.h"
#include <cmath>

using namespace std;

Wall::Wall(int r, int c, Orientation o) : row(r), col(c), orientation(o) {}

bool Wall::conflicts(const Wall& other) const {
    // same anchor means duplicate or perpendicular crossing.
    if (this->row == other.row && this->col == other.col) {
        return true; 
    }

    // horizontal walls conflict when they touch end-to-end on the same row.
    if (this->orientation == Orientation::HORIZONTAL && other.orientation == Orientation::HORIZONTAL) {
        // adjacent columns share one segment and are not both allowed.
        if (this->row == other.row && abs(this->col - other.col) == 1) {
            return true;
        }
    }

    // vertical walls conflict when they touch end-to-end on the same column.
    if (this->orientation == Orientation::VERTICAL && other.orientation == Orientation::VERTICAL) {
        // adjacent rows share one segment and are not both allowed.
        if (this->col == other.col && abs(this->row - other.row) == 1) {
            return true;
        }
    }

    return false; // no conflict found.
}