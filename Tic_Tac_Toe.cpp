// Tic_Tac_Toe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "constants.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

#include "Tic_Tac_Toe.h"



int TicTacToe::run(string x, string o)
{
	//So roughly there should be a communicate with server and communciate with client functions here. We'll be using switch statments to get the user able to interact with the server
	
	// Board theBoard;
	// Referee theRef;
	// Game theGame;
	// string name;

	// printf("\nPlease enter the name of the \'X\' player: ");
	// getline(cin, name);

	xPlayer = Player(x, 'X', &theBoard); 

	// printf("\nPlease enter the name of the \'O\' player: ");
	// getline(cin, name);

	oPlayer = Player(o, 'O', &theBoard);

	cout << oPlayer.getMark() << endl;
	cout << oPlayer.getName() << endl;

	theRef.setBoard(theBoard);
	theRef.setOPlayer(oPlayer);
	theRef.setXPlayer(xPlayer);

	theGame.appointReferee(theRef);
	


	// xPlayer.play();		//begins the game
	return 0;
	
}

string TicTacToe::makeMove(bool xTurn, int col, int row){
	if(xTurn){
		xPlayer.play(col, row);
	}else{
		oPlayer.play(col, row);
	}

	return "turn finished (fix this later)";
}







