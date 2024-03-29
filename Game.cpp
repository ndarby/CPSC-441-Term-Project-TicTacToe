//
// Created by Greg on 2019-11-07.
//
#include "Game.h"

Game::Game() {
    board = new Board();
    currentTurn = xTurn;
}


bool Game::isFull() {
    return (xPlayer != NULL) && (oPlayer != NULL);

}

bool Game::makeMove(int row, int col, Mark mark) {
        if (mark == xMark) {
        if (currentTurn != xTurn) {
            return false;
        }
    } else if (currentTurn != oTurn) {
        return false;
    }

    if (board->getMark(row, col) == emptyMark) {
        board->addMark(row, col, mark);
        switchTurn();
        return true;
    }

    return false;
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

Player *Game::getOpponent(Player *player) {
    if (player == xPlayer) {
        return oPlayer;
    } else {
        return xPlayer;
    }
}

bool Game::checkWin(Player *player) {
    return board->checkWinner(player->getMark());
}




