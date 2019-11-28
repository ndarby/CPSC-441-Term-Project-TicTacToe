#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_PLAYER_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_PLAYER_H

#include <string>
#include "constants.h"
#include "Game.h"

using namespace std;

class User;
class Game;

class Player {
private:
public:
    void setGame(Game *game);

private:
    
    Mark mark;
    Game* game;
    User* user;

public:
    string userName;

    Player(string userName);

    void setMark(Mark mark);

    Mark getMark() const;

    bool play(int col, int row);

    Game *getGame() const;

    User *getUser() const;

    void setUser(User *user);

};

#endif