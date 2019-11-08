#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H

#include <string>
#include "constants.h"

#include "Board.h"
#include "Player.h"

class Player;

class Game {
private:
    Player* xPlayer = NULL;
    Player* oPlayer = NULL;
    Board* board = NULL;
    Turn currentTurn = xTurn;

public:
    Game();

    bool isFull();

    bool makeMove(int row, int col, Mark mark);

    void setPlayer(Player *player);

    string sendState();

    Player *getXPlayer() const;

    Player *getOPlayer() const;

    Player* getOpponent(Player* player);

    bool checkWin(Player* player);

    void switchTurn();
};


#endif //CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
