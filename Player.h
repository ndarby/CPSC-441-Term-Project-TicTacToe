#include "constants.h"
#include "Board.h"

class Player {
protected:
    string name;

    char mark;

    Board *myBoard;

public:

    Player() {}

    Player(string n, char m, Board *b);

    void play(int col, int row);

    void makeMove(int col, int row);

};