//
// Created by Greg on 2019-11-07.
//
#include "Game.h"

Game::Game() {
    board = new Board();
    currentTurn = xTurn;
    xPlayer = NULL;
    oPlayer = NULL;
}


bool Game::isFull() {
    return (xPlayer != NULL) && (oPlayer != NULL);

}

bool Game::makeMove(int row, int col, Mark mark) {
    char charMark;

    if (mark == xPlayer->getMark()) {
        charMark = X_MARK;
    } else {
        charMark = O_MARK;
    }

    if (board->checkWinner(charMark)) {

    }

    if (board->getMark(row, col) == SPACE_CHAR) {
        board->addMark(row, col, charMark);
        switchTurn();
        return true;
    } else {
        return false;
    }
}

void Game::switchTurn() {
    if (currentTurn == oTurn) {
        currentTurn = xTurn;
    } else {
        currentTurn = oTurn;
    }
}

void Game::setPlayer(Player* player) {
    if (!xPlayer) {
        xPlayer = player;
        xPlayer->setMark(xMark);
    } else {
        oPlayer = player;
        oPlayer->setMark(oMark);
    }

    player->setGame(this);
}

string Game::sendState() {
    return board->currentBoard();
}

Player *Game::getXPlayer() const {
    return xPlayer;
}

Player *Game::getOPlayer() const {
    return oPlayer;
}

bool Game::xWins(){
    return board->xWins();
}

bool Game::oWins(){
    return board->oWins();
}

