//
// Created by Greg on 2019-11-07.
//
#include "Game.h"

Game::Game() {
    board = new Board();
    currentTurn = xTurn;
    gameNum = rand() %100; //random number between 0-100
}

Game::Game(int n) {
    board = new Board();
    currentTurn = xTurn;
    gameNum = n; //random number between 0-100
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

    if (mark == xMark) {
        if (currentTurn != xTurn) {
            return false;
        }
    } else if (currentTurn != oTurn) {
        return false;
    }

    if (board->getMark(row, col) == SPACE_CHAR) {
        board->addMark(row, col, charMark);
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

    char charMark;

    if (player->getMark() == xPlayer->getMark()) {
        charMark = X_MARK;
    } else {
        charMark = O_MARK;
    }


    return board->checkWinner(charMark);
}

int Game::getgameNum(void){
    return gameNum;
}
void Game::setgameNum(int num){
    gameNum = num;
}

int Game::howManyPlayers(){
    if((xPlayer != NULL) && (oPlayer != NULL))
        return 2;
    if((xPlayer == NULL) && (oPlayer != NULL))
        return 1;
    if((xPlayer != NULL) && (oPlayer == NULL))
        return 1;
    if((xPlayer == NULL) && (oPlayer == NULL))
        return 0;
}


