// Tic_Tac_Toe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "constants.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>
using namespace std;

#include "Tic_Tac_Toe.h"



int TicTacToe::setup(string x, string o)
{
	theBoard = Board();
	xPlayer = Player(x, 'X', &theBoard); 
	oPlayer = Player(o, 'O', &theBoard);

	
	return 0;
	
}

string TicTacToe::makeMove(bool xTurn, int col, int row){
	if(xTurn){
		xPlayer.play(col, row);
	}else{
		oPlayer.play(col, row);
	}

	xPlayer.displayBoard();

	return "turn finished (fix this later)";
}







