#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using namespace std;
#include "Position.h"

class Player {
private:
    int id;
    Position position;
    int wallsLeft;
    string name;
    int goalRow;

public:
    Player(int id, string name, Position startPos, int goalR, int walls = 10);

    void move(Position newPos);
    bool useWall();
    bool hasReachedGoal() const;
    Position getPosition() const;

    int getId() const;
    string getName() const;
    int getWallsLeft() const;
    int getGoalRow() const;

    void setPosition(Position newPos);
    void setWallsLeft(int walls);
    void setGoalRow(int goal);
    void setName(const string& newName);
};

#endif