//
// Created by Greg on 2019-11-07.
//

#ifndef CPSC_441_TERM_PROJECT_TICTACTOE_SELECTSERVER_H
#define CPSC_441_TERM_PROJECT_TICTACTOE_SELECTSERVER_H
#include <iostream>
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()
#include <string>
#include <vector>
#include <map>

#include "constants.h"
#include "User.h"

class Player;


void initServer(int &, int port);

void processSockets(fd_set);

void sendData(int sock, string data);

string receiveData(int, char[], int &);

ServerCommand processData(string);

void loginUser(int sock, string userName);

void displayLeaderboard(int sock);

void processMove(int sock, string data);

void putUserInGame(int sock);

vector<User> registeredUsers = {
        User("Greg"),
        User("Nathan"),
        User("Auguste"),
        User("Grigory")
};
#endif //CPSC_441_TERM_PROJECT_TICTACTOE_SELECTSERVER_H
