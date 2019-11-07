#pragma once

#include "constants.h"
#include "Board.h"

#define SPACE_CHAR ' '
#define LETTER_O 'O'
#define LETTER_X 'X'

class Player {

protected:
	string name;
	char mark;
	Board* myBoard;
	// Player* opponent;
	//RandomGenerator myRanNum;
	/**
	 * Constructs the Player object with the players name and their mark.
	 * @param name
	 * @param mark
	 */
public:
	Player() {
		
	}
	Player(string n, char m, Board* b) {
		name = n;
		mark = m;
		myBoard = b;
		//myRanNum = new RandomGenerator();
	}
	
	string getName() {
		return name;
	}
	
	char getMark() {
		return mark;
	}
	
	// void setOpponent(Player* p) {
	// 	opponent = p;
	// }

	// void setBoard(Board* b) {
	// 	myBoard = *b;
	// }




	void play(int col, int row){
		if(myBoard->xWins()) {
			myBoard->display();
			printf("\nThe winner is player X\n");
			exit(0);
		}
		else if(myBoard->oWins()) {
			myBoard->display();
			printf("\nThe winner is player O\n");
			exit(0);
		}
		else if(myBoard->isFull()) {
			myBoard->display();
			printf("\nNo winner\n");
			exit(0);
		}
		makeMove(col, row);
		
	}
	/**
	 * Enables a player to input and view their choice in the game within accordance of the
	 * rules while transitioning to the other players turn. It does this by insuring that
	 * no player over-writes another players mark.
	 * @throws NumberFormatException
	 * @throws IOException
	 */
	void makeMove(int col, int row){
		

		if(myBoard->getMark(row, col) == SPACE_CHAR){
			myBoard->addMark(row, col, mark);
		}

		myBoard->display();  //send string back to server eventually FIX THIS
	}

	void displayBoard(){
		myBoard->display();
	}

};