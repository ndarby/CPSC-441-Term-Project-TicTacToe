
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

    Mark getMark(int row, int col);

    char getCharMark(int row, int col);

    void addMark(int row, int col, Mark mark);

    bool checkWinner(Mark mark);

    string currentBoard();

    string ColumnHeaders();

    string Hyphens();

    string Spaces();

    void display();

    void displayColumnHeaders();

    void addHyphens();

    void addSpaces();
    
    void clear();
};


#endif
