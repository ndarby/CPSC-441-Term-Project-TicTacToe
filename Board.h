
#ifndef HEADER
#define HEADER

#include <string>
#include <iostream>

#include "constants.h"

using namespace std;

class Board {

private:
    Mark theBoard[3][3] = {{emptyMark, emptyMark, emptyMark},
                           {emptyMark, emptyMark, emptyMark},
                           {emptyMark, emptyMark, emptyMark}};
    int markCount;

public:
    Board();

    char getMark(int row, int col) {
        return theBoard[row][col];
    }

    bool isFull() {
        return markCount == 9;
    }

    void display() {
        displayColumnHeaders();
        addHyphens();
        for (int row = 0; row < 3; row++) {
            addSpaces();
            cout << "  row " << row << "   ";
            for (int col = 0; col < 3; col++)
                cout << "|  " << getMark(row, col) << "  ";
            printf("|\n");
            addSpaces();
            addHyphens();
        }
    }

    void addMark(int row, int col, Mark mark) {

        theBoard[row][col] = mark;
        markCount++;
    }

    void clear() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                theBoard[i][j] = emptyMark;
        markCount = 0;
    }

    int checkWinner(char mark) {
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
        return result;
    }

    void displayColumnHeaders() {
        printf("          ");
        for (int j = 0; j < 3; j++)
            cout << "|col " << j;
        cout << "\n";
    }

    /**
     * Displays the hyphens present on the board
     */
    void addHyphens() {
        printf("          ");
        for (int j = 0; j < 3; j++)
            printf("+-----");
        printf("+\n");
    }

    /**
     * Insures visually spaces are applied to the board
     */
    void addSpaces() {
        printf("          ");
        for (int j = 0; j < 3; j++)
            printf("|     ");
        printf("|\n");
    }


    string currentBoard() {
        string b;
        b = b + ColumnHeaders();
        b = b + Hyphens();
        for (int row = 0; row < 3; row++) {
            b = b + Spaces();
            b = b + "  row " + to_string(row) + "   ";
            for (int col = 0; col < 3; col++)
                b = b + "|  " + getMark(row, col) + "  ";
            b = b + "|\n";
            b = b + Spaces();
            b = b + Hyphens();
        }
        return b;
    }

    /**
     * returns the column headers with appropriate indexes as a string
     */
    string ColumnHeaders() {
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
    string Hyphens() {
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
    string Spaces() {
        string s;
        s = s + "          ";
        for (int j = 0; j < 3; j++)
            s = s + "|     ";
        s = s + "|\n";
        return s;
    }

};


#endif // !HEADER
