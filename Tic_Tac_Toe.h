

#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H


#include <string>
#include "Referee.h"
#include "Player.h"
#include "Board.h"
#include "Game.h"

using namespace std;

class TicTacToe{

public:
	Player xPlayer;
	Player oPlayer;
	Board theBoard;
	Referee theRef;
	Game theGame;

	TicTacToe(){
		theBoard = Board();
		theRef = Referee();
		theGame = Game();
	}

	int run(string x, string o);

	string makeMove(bool xTurn, int col, int row);

};



#endif