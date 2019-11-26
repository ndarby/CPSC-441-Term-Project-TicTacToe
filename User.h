//
// Created by Greg on 2019-11-07.
//

#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_USER_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_USER_H

#include <string>

#include "constants.h"
#include "Game.h"

class Player;
class Game;

using namespace std;

class User {
private:
    string userName;
    bool isBanned;
    int wins;
    int loses;
    int draws;
    int totalGames;
    int sock;
    int score;
public:
    int getSock() const;

    void setSock(int sock);

private:
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

    void setWins(int w);
    void setLoses(int l);
    void setDraws(int d);
    void setOnlineStatus(bool online);

    int getWins(void);
    int getLoses(void);
    int getDraws(void);

    string returnUsername(void);
    void setUsername(string u);


    void updateUserScore(void);
    int getUserScore(void);

    bool getBannedStatus(void);
    void setBannedStatus(bool status);
};


#endif //CPSC_441_TERM_PROJECT_TICTACTOE_USER_H
