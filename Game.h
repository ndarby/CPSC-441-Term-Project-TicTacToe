#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H

#include <string>
#include "constants.h"

class Player;
class Board;

class Game {
private:
    Player* xPlayer;
    Player* oPlayer;
    Board* board;
    Turn currentTurn;

    void switchTurn();

public:
    Game();

    bool isFull();

    bool makeMove(int row, int col, Mark mark);

    void setPlayer(Player *player);

    string sendState();

    Player *getXPlayer() const;

    Player *getOPlayer() const;
};


#endif //CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
