/*
 * A simple TCP select server that accepts multiple connections and echo message back to the clients
 * For use in CPSC 441 lectures
 * Modified for use in CPSC 441 Term Project
 */

#include "SelectServer.h"
#include "Game.h"
#include "User.h"

using namespace std;

const int BUFFERSIZE = 32;   // Size the message buffers
const int MAXPENDING = 10;    // Maximum pending connections

fd_set recvSockSet;   // The set of descriptors for incoming connections
int maxDesc = 0;      // The max descriptor
bool terminated = false;

vector<Game> activeGames = {
        Game(),

};

std::map<int, User*> activeUsers = map<int, User*>();

vector<User> registeredUsers = {
        User("Greg"),
        User("Nathan"),
        User("Auguste"),
        User("Grigory"),
        User("Bob"),
        User("Alex")
};

int main(int argc, char *argv[]) {

    int TCPSock;                  // server socket descriptor
    int clientSock;                  // client socket descriptor
    struct sockaddr_in clientAddr;   // address of the client

    struct timeval timeout = {0, 10};  // The timeout value for select()
    struct timeval selectTime;
    fd_set tempRecvSockSet;            // Temp. receive socket set for select()
//
//    xPlayersSet.push_back(false);
//    oPlayersSet.push_back(false);
//    allGames.push_back(TicTacToe());    //not sure if we want to use this array list to contain all the games?
//
//    // Check for input errors
    if (argc != 2) {
        cout << "Usage: " << argv[1] << " <Listening Port>" << endl;
        exit(1);
    }

    // Initilize the server
    initServer(TCPSock, atoi(argv[1]));
    cout << "\nServer init successful" << endl;

    // Clear the socket sets    
    FD_ZERO(&recvSockSet);

    // Add the listening socket to the set
    FD_SET(TCPSock, &recvSockSet);
    maxDesc = max(TCPSock, maxDesc);

    // Run the server until a "terminate" command is received)
    while (!terminated) {
        // copy the receive descriptors to the working set
        memcpy(&tempRecvSockSet, &recvSockSet, sizeof(recvSockSet));

        // Select timeout has to be reset every time before select() is
        // called, since select() may update the timeout parameter to
        // indicate how much time was left.
        selectTime = timeout;
        int ready = select(maxDesc + 1, &tempRecvSockSet, NULL, NULL, &selectTime);
        if (ready < 0) {
            cout << "select() failed" << endl;
            break;
        }

        // First, process new connection request, if any.
        if (FD_ISSET(TCPSock, &tempRecvSockSet)) {
            // set the size of the client address structure
            unsigned int size = sizeof(clientAddr);
            // Establish a connection
            if ((clientSock = accept(TCPSock, (struct sockaddr *) &clientAddr, (socklen_t *) &size)) < 0)
                break;
            cout << "Accepted a TCP connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << clientAddr.sin_port
                 << endl;

//            //sets the IP address to either the xPlayer or the oPlayer
//            if (!xPlayerSet) {
//                xPlayerSock = clientSock;
//                xPlayerSet = true;
//            } else if (xPlayerSet && !oPlayerSet) {
//                oPlayerSock = clientSock;
//                oPlayerSet = true;
//                theGame.setup("xPlayer", "oPlayer", xPlayerSock, oPlayerSock); //FIX THIS add usernames later
//            } else {
//                cout << "Player is not able to be added to the game." << endl;
//            }

            // Add the new connection to the receive socket set
            FD_SET(clientSock, &recvSockSet);
            maxDesc = max(maxDesc, clientSock);
        } else {
            processSockets(tempRecvSockSet);
        }
    }

    for (int sock = 0; sock <= maxDesc; sock++) {
        if (FD_ISSET(sock, &recvSockSet)) {
            close(sock);
        }
    }

    close(TCPSock);

}

void processSockets(fd_set readySocks) {
    char *buffer = new char[BUFFERSIZE];       // Buffer for the message from the server
    int size;                                    // Actual size of the message 

    // Loop through the descriptors and process
    string fromClient;
    for (int sock = 0; sock <= maxDesc; sock++) {
        if (!FD_ISSET(sock, &readySocks))
            continue;

        // Clear the buffers
        memset(buffer, 0, BUFFERSIZE);


        fromClient = receiveData(sock, buffer, size);

        ServerCommand command = processData(fromClient);
        string userName;

        switch (command) {
            case login:
                cout << "login command received" << endl;
                userName = fromClient.substr(6, string::npos);
                loginUser(sock, userName);
                break;
            case leaderboard:
                cout << "leaderboard command received" << endl;
                displayLeaderboard(sock);
                break;
            case startgame:
                cout << "startgame command received" << endl;
                putUserInGame(sock);
                break;
            case makeMove:
                cout << "makeMove command received" << endl;
                processMove(sock, fromClient);
                break;
        }

        //sendData(sock, buffer, size);
    }

    delete[] buffer;
}

string receiveData(int sock, char *inBuffer, int &size) {
    // Receive the message from client
    size = recv(sock, (char *) inBuffer, BUFFERSIZE, 0);

    // Check for connection close (0) or errors (< 0)
    if (size <= 0) {
        cout << "recv() failed, or the connection is closed. " << endl;
        FD_CLR(sock, &recvSockSet);

        // Update the max descriptor
        while (FD_ISSET(maxDesc, &recvSockSet) == false)
            maxDesc -= 1;
        return NULL;
    }

    string msg = string(inBuffer);
    cout << "Client: " << msg << endl;

    return msg;
}

ServerCommand processData(string data) {

    cout << "in processData: " << data << endl;
    if (data.find("LEADERBOARD") == 0) {
        return leaderboard;
    } else if (data.find("LOGIN") == 0) {
        return login;
    } else if (data.find("PLAY") == 0) {
        return startgame;
    } else {
        return makeMove;
    }

}

void processMove(int sock, string data) {
    int col = data[0] - '0';
    int row = data[2] - '0';

    User* currentUser = activeUsers[sock];

    if (currentUser->getPlayer()->play(col, row)) { //valid move
        sendData(sock, "MOVE SUCCESS");
        sendData(sock, currentUser->getPlayer()->getGame()->sendState());
    } else {
        sendData(sock, "MOVE FAILED");
    }

}

void putUserInGame(int sock) {
    User* user = activeUsers[sock];
    cout << "found sockets user " << user->getUserName() << endl;
    Player* player = new Player(user->getUserName());   //TODO this probably causes a memory leak
    user->setPlayer(player);

    Game* potentialGame = &activeGames.back();
    if (!potentialGame->isFull()) {
        cout << "found game with a spot" << endl;
        potentialGame->setPlayer(player);
    } else {
        cout << "Made new game" << endl;
        potentialGame = new Game();
        potentialGame->setPlayer(player);
        activeGames.push_back(*potentialGame);
    }
}

void sendData(int sock, string data) {

    cout << "Sending to client: " << data << endl;

    const char *toSend = data.c_str();
    int bytesSent = 0;
    int totalBytesToSend = data.length();
    int messageLength = 0;

    while (totalBytesToSend != 0) {
        if (totalBytesToSend < BUFFERSIZE) {
            messageLength = totalBytesToSend;
        } else {
            messageLength = BUFFERSIZE;
        }

        bytesSent = send(sock, &toSend[bytesSent], messageLength, 0);

        totalBytesToSend -= bytesSent;
    }

}


void loginUser(int sock, string userName) {
    bool userLoggedIn = false;
    for (int i = 0; i <= registeredUsers.size(); i++) {
        if ((registeredUsers[i].getUserName() == userName) && registeredUsers[i].attemptLogin()) {
            sendData(sock, "LOGIN SUCCESS");
            userLoggedIn = true;
            activeUsers[sock] = &registeredUsers[i];
            break;
        }
    }
    if (!userLoggedIn) {
        sendData(sock, "LOGIN FAILED");
    }
}

void displayLeaderboard(int sock) {
    //TODO retrieve leaderboard
    sendData(sock, "SOME SORT OF LEADERBOARD");
}


void initServer(int &TCPSock, int port) {
    struct sockaddr_in serverAddr;   // address of the server

    // Create a TCP socket
    // * AF_INET: using address family "Internet Protocol address"
    // * SOCK_STREAM: Provides sequenced, reliable, bidirectional, connection-mode byte streams.
    // * IPPROTO_TCP: TCP protocol
    if ((TCPSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        cout << "socket() failed" << endl;
        exit(1);
    }

    // Free up the port before binding
    // * sock: the socket just created
    // * SOL_SOCKET: set the protocol level at the socket level
    // * SO_REUSEADDR: allow reuse of local addresses
    // * &yes: set SO_REUSEADDR on a socket to true (1)
    // * sizeof(int): size of the value pointed by "yes"
    int yes = 1;
    if (setsockopt(TCPSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        cout << "setsockopt() failed" << endl;
        exit(1);
    }

    // Initialize the server information
    // Note that we can't choose a port less than 1023 if we are not privileged users (root)
    memset(&serverAddr, 0, sizeof(serverAddr));         // Zero out the structure
    serverAddr.sin_family = AF_INET;                    // Use Internet address family
    serverAddr.sin_port = htons(port);                  // Server port number
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);     // Any incoming interface

    // Bind to the local address
    if (bind(TCPSock, (sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        cout << "bind() failed" << endl;
        exit(1);
    }

    // Listen for connection requests
    if (listen(TCPSock, MAXPENDING) < 0) {
        cout << "listen() failed" << endl;
        exit(1);
    }
}
