#include "Player.h"
using namespace std;

Player::Player(int id, string name, Position startPos, int goalR, int walls)
    : id(id), position(startPos), wallsLeft(walls), name(name), goalRow(goalR) {}

void Player::move(Position newPos) {
    position = newPos;
}

bool Player::useWall() {
    if (wallsLeft > 0) {
        wallsLeft--;
        return true;
    }
    return false; // no walls left to place.
}

bool Player::hasReachedGoal() const {
    return position.row == goalRow;
}

Position Player::getPosition() const {
    return position;
}

int Player::getId() const {
    return id;
}

string Player::getName() const {
    return name;
}

int Player::getWallsLeft() const {
    return wallsLeft;
}

int Player::getGoalRow() const {
    return goalRow;
}

void Player::setPosition(Position newPos) {
    position = newPos;
}

void Player::setWallsLeft(int walls) {
    wallsLeft = walls;
}

void Player::setGoalRow(int goal) {
    goalRow = goal;
}

void Player::setName(const string& newName) {
    name = newName;
}