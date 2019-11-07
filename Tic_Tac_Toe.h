

#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H


#include <string>
#include "Player.h"
#include "Board.h"

using namespace std;

class TicTacToe{

public:
	Player xPlayer;
	Player oPlayer;
	Board theBoard;

	TicTacToe(){
	}

	int setup(string x, string o);

	string makeMove(bool xTurn, int col, int row);

};



#endif