//
// Created by Greg on 2019-11-07.
//

#include "User.h"

const string &User::getUserName() const {
    return userName;
}

User::User(string userName) {
    isBanned = false;
    currentlyOnline = false;

    this->userName = userName;
    // wins = ((int64_t) &wins) % 11;      //dont do this at home
    // loses = ((int64_t) &loses) % 11;
    // draws = ((int64_t) &draws) % 5;
    wins = 0;
    loses = 0;
    draws = 0;

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

void User::setOnlineStatus(bool online){
    currentlyOnline = online;
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
    player->setUser(this);
}

int User::getSock() const {
    return sock;
}

void User::setSock(int sock) {
    User::sock = sock;
}

void User::setWins(int w){
    wins = w;
}
void User::setLoses(int l){
    loses = l;
}
void User::setDraws(int d){
    draws = d;
}


int User::getWins(){
    return wins;
}
int User::getLoses(){
    return loses;
}
int User::getDraws(){
    return draws;
}

string User::returnUsername(){
    return userName;
}

void User::setUsername(string u){
    userName = u;
}

void User::updateUserScore(){
    score = (2*wins) - loses;
}

int User::getUserScore(){
    return score;
}

bool User::getBannedStatus(){
    return isBanned;
}
void User::setBannedStatus(bool status){
    isBanned = status;
}
