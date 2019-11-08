

#include "Player.h"

Player::Player(string n, char m, Board *b) {
    name = n;
    mark = m;
    myBoard = b;
}

void Player::play(int col, int row) {
    if (myBoard->xWins()) {
        myBoard->display();
        printf("\nThe winner is player X\n");
        exit(0);
    } else if (myBoard->oWins()) {
        myBoard->display();
        printf("\nThe winner is player O\n");
        exit(0);
    } else if (myBoard->isFull()) {
        myBoard->display();
        printf("\nNo winner\n");
        exit(0);
    }

    makeMove(col, row);
}

void Player::makeMove(int col, int row) {

    if (myBoard->getMark(row, col) == SPACE_CHAR) {
        myBoard->addMark(row, col, mark);
    }

    myBoard->display();  //send string back to server eventually FIX THIS
}
