//
// Created by Greg on 2019-11-07.
//

#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_USER_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_USER_H

#include <string>

#include "Game.h"
#include "constants.h"

class Player;

using namespace std;

class User {
private:
    string userName;
    int wins;
    int loses;
    int draws;
    int totalGames;
    bool currentlyOnline;

public:
    Player *getPlayer() const;

    void setPlayer(Player *player);

private:
    Player *player;

public:
    User(string userName);

    void changeRecord(GameResult result);

    bool attemptLogin();

    const string &getUserName() const;

    bool isCurrentlyOnline() const;
};


#endif //CPSC_441_TERM_PROJECT_TICTACTOE_USER_H
