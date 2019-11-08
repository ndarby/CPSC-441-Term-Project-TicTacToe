//
// Created by Greg on 2019-11-07.
//

#include "User.h"


const string &User::getUserName() const {
    return userName;
}

User::User(string userName) {
    
    currentlyOnline = false;
    
    this->userName = userName;
    wins = ((int64_t) &wins) % 11;      //dont do this at home
    loses = ((int64_t) &loses) % 11;
    draws = ((int64_t) &draws) % 5;

    totalGames = (wins + loses + draws);
}

void User::changeRecord(GameResult result) {
    switch (result) {
        case win:
            wins++;
            break;
        case loss:
            loses++;
            break;
        case draw:
            draws++;
            break;
    }

    totalGames++;
}

bool User::isCurrentlyOnline() const {
    return currentlyOnline;
}

bool User::attemptLogin() {
    if (!currentlyOnline) {
        currentlyOnline = true;
        return true;
    }

    return false;
}

Player *User::getPlayer() const {
    return player;
}

void User::setPlayer(Player *player) {
    User::player = player;
}


