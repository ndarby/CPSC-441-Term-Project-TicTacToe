#pragma once
#include "constants.h"
#include "Game.h"
#include "Referee.h"
#include "Player.h"
#include "Board.h"






class Referee {
private:
	Board theBoard;
	Player xPlayer;
	Player oPlayer;

public:

	Referee() {
	}

	void setBoard(Board b) {
		theBoard = b;
	}

	void setOPlayer(Player p) {
		oPlayer = p;
	}

	void setXPlayer(Player p) {
		xPlayer = p;
	}

	void runTheGame() {
		xPlayer.setOpponent(&oPlayer);
		oPlayer.setOpponent(&xPlayer);

		theBoard.display();
		// xPlayer.play();
	}

};
