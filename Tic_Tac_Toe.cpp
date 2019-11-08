#include <string>

#include "constants.h"

using namespace std;

#include "Tic_Tac_Toe.h"

int TicTacToe::setup(string x, string o, int xs, int os)
{
	theBoard = Board();
	xPlayer = Player(x, X_MARK, &theBoard);
	oPlayer = Player(o, O_MARK, &theBoard);

	xSock = xs;
	oSock = os;
	xPlayerTurn = true;

	return 0;
	
}

string TicTacToe::makeMove(bool xTurn, int col, int row){
	if(xTurn){
		xPlayer.play(col, row);
	}else{
		oPlayer.play(col, row);
	}

	return theBoard.currentBoard();
}







