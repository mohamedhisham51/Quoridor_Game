#include "PathFinder.h"

static const char kDirs[4] = {'U', 'D', 'L', 'R'};
static const int kDr[4] = {-1, 1, 0, 0};
static const int kDc[4] = {0, 0, -1, 1};

bool PathFinder::bfs(const GameBoard& board, Position start, int goalRow) {
    int size = board.getBoardSize();
    if (!start.isValid(size)) {
        return false;
    }

    // standard bfs frontier and visited set.
    vector<vector<bool>> visited(size, vector<bool>(size, false));
    queue<Position> q;
    q.push(start);
    visited[start.row][start.col] = true;

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        if (cur.row == goalRow) {
            // first hit proves at least one valid path exists.
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            if (!board.isValidMove(cur, kDirs[i])) {
                continue;
            }
            Position next(cur.row + kDr[i], cur.col + kDc[i]);
            if (!next.isValid(size)) {
                continue;
            }
            if (!visited[next.row][next.col]) {
                visited[next.row][next.col] = true;
                q.push(next);
            }
        }
    }

    return false;
}

int PathFinder::shortestPath(const GameBoard& board, Position start, int goalRow) {
    int size = board.getBoardSize();
    if (!start.isValid(size)) {
        return -1;
    }

    // dist grid doubles as visited marker for shortest path bfs.
    vector<vector<int>> dist(size, vector<int>(size, -1));
    queue<Position> q;
    q.push(start);
    dist[start.row][start.col] = 0;

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        if (cur.row == goalRow) {
            return dist[cur.row][cur.col];
        }

        for (int i = 0; i < 4; ++i) {
            if (!board.isValidMove(cur, kDirs[i])) {
                continue;
            }
            Position next(cur.row + kDr[i], cur.col + kDc[i]);
            if (!next.isValid(size)) {
                continue;
            }
            if (dist[next.row][next.col] == -1) {
                dist[next.row][next.col] = dist[cur.row][cur.col] + 1;
                q.push(next);
            }
        }
    }

    return -1;
}
