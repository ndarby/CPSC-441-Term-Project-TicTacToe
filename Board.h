
#ifndef HEADER
#define HEADER
#include "constants.h"


class Board {       // The class

private:
	char theBoard[3][3] = { { ' ', ' ', ' '},
							{ ' ', ' ', ' '},
							{ ' ', ' ', ' '} };
	int markCount;

public:             // Access specifier
	Board() {
		markCount = 0;
	}


	char getMark(int row, int col) {
		return theBoard[row][col];
	}
	/**
	 * Checks if the board is full
	 * @return true if the board is full
	 */
	bool isFull() {
		return markCount == 9;
	}
	/**
	 * Calls checkWinner(char) to see if this specific player won the game
	 * @return True if 'X' won the game, otherwise returns false
	 */
	bool xWins() {
		if (checkWinner(LETTER_X) == 1)
			return true;
		else
			return false;
	}
	/**
	 * Calls checkWinner(char) to see if this specific player won the game
	 * @return true if O won the game
	 */
	bool oWins() {
		if (checkWinner(LETTER_O) == 1)
			return true;
		else
			return false;
	}
	/**
	 * Displays the boards rows & columns with the appropriate characters
	 */
	void display() {
		displayColumnHeaders();
		addHyphens();
		for (int row = 0; row < 3; row++) {
			addSpaces();
			cout <<"  row " << row << "   ";
			for (int col = 0; col < 3; col++)
				cout<< "|  " << getMark(row, col)<< "  ";
			printf("|\n");
			addSpaces();
			addHyphens();
		}
	}
	/**
	 * Places a players character on the board.
	 * Additionally updates the total amount of characters
	 * @param row
	 * @param col
	 * @param mark
	 */
	void addMark(int row, int col, char mark) {

		theBoard[row][col] = mark;
		markCount++;
	}
	void removeMark(int row, int col, char mark) {
		theBoard[row][col] = ' ';
		markCount--;
	}
	/**
	 * Clears the board of characters.
	 * Additionally resets the tally of characters present
	 */
	void clear() {
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				theBoard[i][j] = SPACE_CHAR;
		markCount = 0;
	}
	/**
	 * Checks for a win condition present on the board for a character.
	 * Will check for 3 horizontal, 3 vertical, and 2 diagonal wins.
	 * @param mark
	 * @return 1 if the character checked won the game
	 */
	int checkWinner(char mark) {
		int row, col;
		int result = 0;

		for (row = 0; result == 0 && row < 3; row++) {
			int row_result = 1;
			for (col = 0; row_result == 1 && col < 3; col++)
				if (theBoard[row][col] != mark)
					row_result = 0;
			if (row_result != 0)
				result = 1;
		}


		for (col = 0; result == 0 && col < 3; col++) {
			int col_result = 1;
			for (row = 0; col_result != 0 && row < 3; row++)
				if (theBoard[row][col] != mark)
					col_result = 0;
			if (col_result != 0)
				result = 1;
		}

		if (result == 0) {
			int diag1Result = 1;
			for (row = 0; diag1Result != 0 && row < 3; row++)
				if (theBoard[row][row] != mark)
					diag1Result = 0;
			if (diag1Result != 0)
				result = 1;
		}
		if (result == 0) {
			int diag2Result = 1;
			for (row = 0; diag2Result != 0 && row < 3; row++)
				if (theBoard[row][3 - 1 - row] != mark)
					diag2Result = 0;
			if (diag2Result != 0)
				result = 1;
		}
		return result;
	}
	/**
	 * Displays the column headers with appropriate indexes
	 */
	void displayColumnHeaders() {
		printf("          ");
		for (int j = 0; j < 3; j++)
			cout<<"|col " <<j;
		cout<< "\n" ;
	}
	/**
	 * Displays the hyphens present on the board
	 */
	void addHyphens() {
		printf("          ");
		for (int j = 0; j < 3; j++)
			printf("+-----");
		printf("+\n");
	}
	/**
	 * Insures visually spaces are applied to the board
	 */
	void addSpaces() {
		printf("          ");
		for (int j = 0; j < 3; j++)
			printf("|     ");
		printf("|\n");
	}
};



#endif // !HEADER
