//
// Created by Greg on 2019-11-08.
//
#include "Board.h"

Board::Board() {
    markCount = 0;
}

Mark Board::getMark(int row, int col) {
    return theBoard[row][col];
}

char Board::getCharMark(int row, int col) {
    if (theBoard[row][col] == xMark) {
        return X_MARK;
    } else {
        return O_MARK;
    }
}

void Board::display() {
    displayColumnHeaders();
    addHyphens();
    for (int row = 0; row < 3; row++) {
        addSpaces();
        cout << "  row " << row << "   ";
        for (int col = 0; col < 3; col++)
            cout << "|  " << getCharMark(row, col) << "  ";
        printf("|\n");
        addSpaces();
        addHyphens();
    }

}

void Board::addMark(int row, int col, Mark mark) {
    theBoard[row][col] = mark;
    markCount++;
}

void Board::clear() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            theBoard[i][j] = emptyMark;
    markCount = 0;
}

bool Board::checkWinner(Mark mark) {
    int row, col;
    int result = 0;

    for (row = 0; result == 0 && row < 3; row++) {
        int row_result = 1;
        for (col = 0; row_result == 1 && col < 3; col++)
            if (theBoard[row][col] != mark)
                row_result = 0;
        if (row_result != 0)
            result = 1;
    }


    for (col = 0; result == 0 && col < 3; col++) {
        int col_result = 1;
        for (row = 0; col_result != 0 && row < 3; row++)
            if (theBoard[row][col] != mark)
                col_result = 0;
        if (col_result != 0)
            result = 1;
    }

    if (result == 0) {
        int diag1Result = 1;
        for (row = 0; diag1Result != 0 && row < 3; row++)
            if (theBoard[row][row] != mark)
                diag1Result = 0;
        if (diag1Result != 0)
            result = 1;
    }
    if (result == 0) {
        int diag2Result = 1;
        for (row = 0; diag2Result != 0 && row < 3; row++)
            if (theBoard[row][3 - 1 - row] != mark)
                diag2Result = 0;
        if (diag2Result != 0)
            result = 1;
    }

    return result == 1;
}

void Board::addHyphens() {
    printf("          ");
    for (int j = 0; j < 3; j++)
        printf("+-----");
    printf("+\n");
}

/**
 * Insures visually spaces are applied to the board
 */
void Board::addSpaces() {
    printf("          ");
    for (int j = 0; j < 3; j++)
        printf("|     ");
    printf("|\n");
}


string Board::currentBoard() {
    string b;
    b = b + ColumnHeaders();
    b = b + Hyphens();
    for (int row = 0; row < 3; row++) {
        b = b + Spaces();
        b = b + "  row " + to_string(row) + "   ";
        for (int col = 0; col < 3; col++)
            b = b + "|  " + getCharMark(row, col) + "  ";
        b = b + "|\n";
        b = b + Spaces();
        b = b + Hyphens();
    }
    return b;
}

/**
 * returns the column headers with appropriate indexes as a string
 */
string Board::ColumnHeaders() {
    string c;
    c = c + "          ";
    for (int j = 0; j < 3; j++)
        c = c + "|col " + to_string(j);
    c = c + "\n";
    return c;
}

/**
 * returns the hyphens present on the board as a string
 */
string Board::Hyphens() {
    string h;
    h = h + "          ";
    for (int j = 0; j < 3; j++)
        h = h + "+-----";
    h = h + "+\n";
    return h;
}

/**
 * Insures visually spaces are applied to the board
 */
string Board::Spaces() {
    string s;
    s = s + "          ";
    for (int j = 0; j < 3; j++)
        s = s + "|     ";
    s = s + "|\n";
    return s;
}

void Board::displayColumnHeaders() {
    printf("          ");
    for (int j = 0; j < 3; j++)
        cout << "|col " << j;
    cout << "\n";
}