#include <string>
#include "Game.h"

using namespace std;

class Game;

class Player {
private:
public:
    void setGame(Game *game);

private:
    string userName;
    Mark mark;
    Game* game;

public:
    Player(string userName);

    void setMark(Mark mark);

    Mark getMark() const;

    bool play(int col, int row);

};