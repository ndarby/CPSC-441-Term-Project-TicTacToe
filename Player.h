#include <string>
#include "constants.h"


using namespace std;

class User;

class Player {
private:
public:
    void setGame(Game *game);

private:
    string userName;
    Mark mark;
    Game* game;
    User* user;

public:
    Player(string userName);

    void setMark(Mark mark);

    Mark getMark() const;

    bool play(int col, int row);

    Game *getGame() const;

    User *getUser() const;

    void setUser(User *user);

};