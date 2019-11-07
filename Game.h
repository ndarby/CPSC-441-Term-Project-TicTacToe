#pragma once
#ifndef GAME
#define GAME

#include "constants.h"
#include "Game.h"
#include "Referee.h"
#include "Player.h"
#include "Board.h"


class Game {

private:
		Board theBoard;
		Referee theRef;
public:

	Game() {
		//no initilization as C++ does this automatically
	}


	void appointReferee(Referee r){
		theRef = r;
		theRef.runTheGame();
	}
	//TODO
	
};


#endif // !GAME
