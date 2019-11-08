#include "Player.h"

Player::Player(std::string userName) {
    this->userName = userName;
    this->mark = mark;
}

bool Player::play(int col, int row) {
    return game->makeMove(row, col, mark);
}

Mark Player::getMark() const {
    return mark;
}

void Player::setMark(Mark mark) {
    Player::mark = mark;
}

void Player::setGame(Game *game) {
    Player::game = game;
}




