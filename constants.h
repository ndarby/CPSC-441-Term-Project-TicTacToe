#ifndef CONSTANTS
#define CONSTANTS

#include <cstddef>

using namespace std;

#define SPACE_CHAR ' '
#define O_MARK 'O'
#define X_MARK 'X'

enum ServerCommand {
    login, leaderboard, makeMove, startgame
};

enum GameResult {
    win, loss, draw
};

enum Mark {
    xMark, oMark
};

enum Turn {
    xTurn, oTurn
};

#include "Board.h"
#include "Game.h"
#include "User.h"
#include "Player.h"
#include "SelectServer.h"

#endif // !CONSTANTS
