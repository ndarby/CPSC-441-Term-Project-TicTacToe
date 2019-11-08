#ifndef CONSTANTS
#define CONSTANTS

#include <cstddef>

#define SPACE_CHAR ' '
#define O_MARK 'O'
#define X_MARK 'X'

using namespace std;

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
#endif // !CONSTANTS
