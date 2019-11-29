#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H

#include <string>
#include "constants.h"
#include <cstdlib>
#include "Board.h"
#include "Player.h"
#include "User.h"

class Player;

class Game {
private:
    int gameNum;
    Player* xPlayer = NULL;
    Player* oPlayer = NULL;
    Board* board = NULL;
    Turn currentTurn = xTurn;

public:

    int observercount = 0;

    User* observerArray[10];

    Game();

    Game(int num);

    bool isFull();

    bool makeMove(int row, int col, Mark mark);

    void setPlayer(Player *player);

    string sendState();

    Player *getXPlayer() const;

    Player *getOPlayer() const;

    Player* getOpponent(Player* player);

    bool checkWin(Player* player);

    void switchTurn();

    int howManyPlayers();

    int getgameNum(void);
    void setgameNum(int num);
};


#endif //CPSC_441_TERM_PROJECT_TICTACTOE_GAME_H
