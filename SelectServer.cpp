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
volatile bool terminated = false;

string leaderBoard;

vector<Game> activeGames = {
        Game(),
        Game(),
        Game(),

};

std::map<int, User*> activeUsers = map<int, User*>();

vector<User> registeredUsers = {
        User("Greg"),
        User("Nathan"),
        User("August"),
        User("Grigory"),
        User("Bob"),
        User("Alex"),
        User("Admin")
};

int main(int argc, char *argv[]) {
    int TCPSock;                  // server socket descriptor
    int clientSock;                  // client socket descriptor
    struct sockaddr_in clientAddr;   // address of the client

    struct timeval timeout = {0, 10};  // The timeout value for select()
    struct timeval selectTime;
    fd_set tempRecvSockSet;            // Temp. receive socket set for select()


    loadUserDetails();
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
            //checkTerminate();                               //Will check to see if the server admin wants to termiante 
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

void joinALiveRoom(int roomNum, int sock, string username){
    User* user = activeUsers[sock];         //curent user
    int positionInActiveGames = 0;
    //cout << "found sockets user " << user->getUserName() << endl;
    Player* player = new Player(username);   //TODO this probably causes a memory leak
    user->setPlayer(player);
    cout << "created player class with name " << username;

    for(int i =0; i<activeGames.size(); i++){           //Has to find the position in active games array that match the roomnumber provided
        if(activeGames.at(i).getgameNum() == roomNum){
            positionInActiveGames = i;
        }
    }

    Game& potentialGame = activeGames[positionInActiveGames];                 //potential game points to the room the user want to join
    int i = 1;
    while (potentialGame.isFull()) {
        cout << "The game room is full! Will assign the player to the next avaliable room";
        potentialGame = activeGames[i];
        i++;
    }

    potentialGame.setPlayer(player);

    if (potentialGame.isFull()) {
        sendData(potentialGame.getOPlayer()->getUser()->getSock(), "WAIT");

        sendData(potentialGame.getXPlayer()->getUser()->getSock(), potentialGame.sendState());
    }
}

string createNewGameRoom(int roomNum){
    for(int i = 0; i<activeGames.size(); i++){
        if(activeGames.at(i).getgameNum() == roomNum)
            return "That game room has a player in it. Can not delete\n";
        else{
            activeGames.push_back(Game(roomNum));
            return listOfRunningGames();
        }
    } 
}

string removeAPlayableRoom(int roomNum){
    for(int i = 0; i<activeGames.size(); i++){
        if(activeGames.at(i).getgameNum() == roomNum)
            if(activeGames.at(i).howManyPlayers() != 0){    //someone is in the game room
                return "That game room has a player in it. Can not delete\n";
            }
            else{
            activeGames.erase(activeGames.begin()+i);
            return listOfRunningGames();
        }
    }
}

string listOfRunningGames(){
    string runningGames;
    runningGames.append("Currently running games:\n");
    for(int i = 0; i<activeGames.size();i++){
        runningGames.append("Game room " + to_string(activeGames.at(i).getgameNum()) + " has " + to_string(activeGames.at(i).howManyPlayers()) + "/2 active players\n");
    }
    return runningGames;
}

string listUsers(){
    string usersOnline;
    usersOnline.append("Users currently online: ");
     for(int j = 0; j <registeredUsers.size(); j++){
        if(registeredUsers.at(j).isCurrentlyOnline())
            usersOnline.append(registeredUsers.at(j).getUserName() + " ");
     }   
     usersOnline.append("\n");  
     return usersOnline;
}

void loadUserDetails(){                             //Loads all the user data stored in a text file 
    ifstream myfile("UserInformation.txt");
    string userInfo;
    //myfile.open("UserInformation.txt");
    int pos = 0;

    while(!myfile.eof()){
        getline(myfile, userInfo);
        
        for(int i = 0; i <registeredUsers.size(); i++){
            
            if(userInfo.substr(0,userInfo.find(" ")) == registeredUsers.at(i).getUserName()){
                pos = userInfo.find(" ");
                registeredUsers.at(i).setUsername(userInfo.substr(0, pos));
                //cout << registeredUsers.at(i).getUserName();
                userInfo.erase(0, pos + 1);

                pos = userInfo.find(" ");
                registeredUsers.at(i).setWins(stoi(userInfo.substr(0, pos)));
                //cout << registeredUsers.at(i).getWins();
                userInfo.erase(0, pos + 1);

                pos = userInfo.find(" ");
                registeredUsers.at(i).setLoses(stoi(userInfo.substr(0, pos)));
                //cout << stoi(userInfo.substr(0, pos));
                userInfo.erase(0, pos + 1);

                pos = userInfo.find(" ");
                registeredUsers.at(i).setDraws(stoi(userInfo.substr(0, pos)));
                //cout << stoi(userInfo.substr(0, pos));
                userInfo.erase(0, pos + 1);

                pos = userInfo.find(" ");                                   //Setting banned status
                if(stoi(userInfo.substr(0, pos)) == 1){
                    registeredUsers.at(i).setBannedStatus(true);
                }
                else{
                    registeredUsers.at(i).setBannedStatus(false);
                }
                //cout << stoi(userInfo.substr(0, pos));
                userInfo.erase(0, pos + 1);
               
            }
        }  
    }
}

void beginTerminate(){
    ofstream myfile;
    string userInfo;
    myfile.open("UserInformation.txt");
    cout << sizeof(registeredUsers);
    for(int i = 0; i <registeredUsers.size(); i++){
        userInfo.append(registeredUsers[i].returnUsername() + " ");
        userInfo.append(to_string((int)registeredUsers[i].getWins())+ " ");
        userInfo.append(to_string((int)registeredUsers[i].getLoses())+ " ");
        userInfo.append(to_string((int)registeredUsers[i].getDraws())+ " ");

        if(registeredUsers[i].getBannedStatus() ==true){
            userInfo.append("1 ");
        }else{
           userInfo.append("0 "); 
        }
        userInfo.append("\n");
    }
    myfile << userInfo;
    myfile.close();
    cout <<"\nUser data has been saved in text file UserInformation.txt in your running directory\n";
    terminated = true;
}

void updateLeaderBoard(){           //Does not account for tied winners! Will randomly pick one and wont mention that fact
    string temp_string;
    for(int i = 0; i <registeredUsers.size(); i++){
        registeredUsers.at(i).updateUserScore();
    }
 
    int ranking [registeredUsers.size()];
    int temp[registeredUsers.size()];

    for(int i = 0; i<registeredUsers.size(); i++){
        temp[i] = registeredUsers.at(i).getUserScore();
        ranking[i] = 0;
    }

    //Credit for this sorting algorithm https://www.geeksforgeeks.org/insertion-sort/
    int i, hold, j;  
    for (i = 1; i < sizeof(temp)/sizeof(int); i++) {  //sorting array of scores
        hold = temp[i];
        j = i - 1;  
        while (temp[j] > hold && j >= 0) {  
            temp[j + 1] = temp[j]; 
            j = j - 1;  
        }  
        temp[j + 1] = hold;  
    }
    ////////////////////////////////////////////////////////////////////////////////
    int values [registeredUsers.size()];
    for (int i = 0, j = registeredUsers.size(); i < registeredUsers.size(); i++, j--){  //matching sorted array score with players
        values[j] = temp[i];
    } 
    


    temp_string.append("CPSC_441 LEADERBOARD!\n");
    for(int j = 0; j <=registeredUsers.size(); j++){
        for(int i = 0; i <registeredUsers.size(); i++){
            if(values[j] == registeredUsers.at(i).getUserScore() && registeredUsers.at(i).returnUsername() != "Admin"){
                if(j == 1){
                        temp_string.append("The current leader on the leaderboard is " + registeredUsers.at(i).getUserName() + " with " + to_string(registeredUsers.at(i).getWins()));
                        temp_string.append(" wins, " + to_string(registeredUsers.at(i).getLoses()) + " loses, and " + to_string(registeredUsers.at(i).getDraws()) + " draws for a total score ");
                        temp_string.append("of " + to_string(registeredUsers.at(i).getUserScore()) + "! Congrats!\n" );
                }else if(j == registeredUsers.size()){
                        temp_string.append("And the big loser is " + registeredUsers.at(i).getUserName() + " who is ranked " + to_string(i+1) + " with " + to_string(registeredUsers.at(i).getWins()));
                        temp_string.append(" wins, " + to_string(registeredUsers.at(i).getLoses()) + " loses, and " + to_string(registeredUsers.at(i).getDraws()) + " draws for a total score");
                        temp_string.append(" of " + to_string(registeredUsers.at(i).getUserScore()) + "! Horrible job!\n" );    
                    }
                else{
                    temp_string.append("Next is " + registeredUsers.at(i).getUserName() + " who is ranked " + to_string(i+2) + " with " + to_string(registeredUsers.at(i).getWins()));
                    temp_string.append(" wins, " + to_string(registeredUsers.at(i).getLoses()) + " loses, and " + to_string(registeredUsers.at(i).getDraws()) + " draws for a total score");
                    temp_string.append(" of " + to_string(registeredUsers.at(i).getUserScore()) + "! Congrats!\n" );
                }
            }
        }
    }
    cout << temp_string << endl;
    leaderBoard.clear(); 
    leaderBoard.append(temp_string); //deep copy  
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
      
        //cout << endl << "PRINTING USERNAME" << userName << endl;

        switch (command) {
            case login:
                cout << "login command received" << endl;
                userName = fromClient.substr(6, string::npos);
                loginUser(sock, userName);
                break;
            case leaderboard:
                cout << "leaderboard command received" << endl;
                updateLeaderBoard();
                sendData(sock, leaderBoard);
                break;
            case startgame:
                cout << "startgame command received" << endl;
                putUserInGame(sock);
                break;
            case makeMove:
                cout << "makeMove command received" << endl;
                processMove(sock, fromClient);
                break;
            case killserver:
                cout << "server shutting down" << endl;
                beginTerminate();
                break;    
            case listusers:
                cout<< "Listing all users\n";
                sendData(sock,listUsers());    
                break;
            case createroom:
                cout<< "creating game room\n";
                sendData(sock, createNewGameRoom(stoi(receiveData(sock,buffer,size))));
                //activeGames.push_back(Game());
               // sendData(sock,listOfRunningGames());
                break;
            case joinroom:
                cout<< "User joining room\n";
                memset(buffer, 0, BUFFERSIZE);
                joinALiveRoom(stoi(receiveData(sock, buffer, size)), sock, activeUsers[sock]->returnUsername());   //recieves the room number the user wants to join
                //sendData(sock,listUsers());    
                break;
            case deleteroom:
                cout<< "Listing all users\n";
                memset(buffer, 0, BUFFERSIZE);
                sendData(sock, removeAPlayableRoom(stoi(receiveData(sock, buffer, size))));  //recieves the user's room number, stoi makes it int, removeAPl.. deletes a room   
                break;                                                                      //and returns an updated list or tells the user the room has someone in it
            case banuser:
                cout<< "Banning user\n";
                memset(buffer, 0, BUFFERSIZE);
                sendData(sock, banAUser(receiveData(sock, buffer, size)));
                break;

            case observeGame:
                cout<< "observing game\n";
                memset(buffer, 0, BUFFERSIZE);
                //cout << activeUsers[sock]->returnUsername();
                //cout << stoi(receiveData(sock, buffer, size));
                joinObserverRoom(stoi(receiveData(sock, buffer, size)), activeUsers[sock]->returnUsername());
                //sendData(sock, banAUser(receiveData(sock, buffer, size)));
                break;
        }

        //sendData(sock, buffer, size);
    }

    delete[] buffer;
}



string banAUser(string username){
    for(int i = 0; i<registeredUsers.size(); i++){
        if(registeredUsers.at(i).returnUsername() == username){
            registeredUsers.at(i).setBannedStatus(true);
        }
    }
    return "User has been banned\n";
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
    } 
    else if (data.find("KILLSERVER") == 0) {
        return killserver;
    }
    else if (data.find("LISTUSERS") == 0) {
        return listusers;
    }
    else if (data.find("CREATEROOM") == 0) {
        return createroom;
    }
    else if (data.find("JOINROOM") == 0) {
        return joinroom;
    }
    else if (data.find("DELETEROOM") == 0) {
        return deleteroom;
    }
    else if (data.find("BANUSER") == 0) {
        return banuser;
    }
    else if (data.find("OBSERVE") == 0) {
        return observeGame;
    }
    else {
        return makeMove;
    }

}

void setWinsandLoses(string winner, string loser){
    for(int i = 0; i<registeredUsers.size(); i++){
        if(registeredUsers.at(i).returnUsername() == winner){
            registeredUsers.at(i).setWins(registeredUsers.at(i).getWins() + 1);
        }
        else if(registeredUsers.at(i).returnUsername() == loser){
              registeredUsers.at(i).setLoses(registeredUsers.at(i).getLoses() + 1);
        }
    }
}

void joinObserverRoom(int roomNum, string username){
    for(int i = 0; i<activeGames.size(); i++){
        if(activeGames.at(i).getgameNum() == roomNum){
            for(int j = 0; j<registeredUsers.size(); j++){
                if(registeredUsers.at(j).returnUsername() == username){
                    activeGames.at(i).observerArray[activeGames.at(i).observercount] = &registeredUsers.at(j);
                    activeGames.at(i).observercount++;
                    cout << "\nHere\n\n";
                }
            }
        }
    }
}

void observe(int gameNum, string gamestate, char mark){
    //cout << "\nHere\n\n";
    for(int i = 0; i<activeGames.size(); i++){
        //cout << "AHHHH\n";
        if(activeGames.at(i).getgameNum() == gameNum)
            for(int k = 0; k<activeGames.at(i).observercount; k++){
                cout << "Sending to client " << gameNum << " \n" << gamestate;
                string send;
                if(mark == 'X'){
                    send.append(activeGames.at(i).getXPlayer()->userName + "'s turn below:\n");
                }
                else if (mark == 'O'){
                    send.append(activeGames.at(i).getOPlayer()->userName + "'s turn below:\n");
                }
                else if (mark == 'F'){      //Game is over
                     sendData(activeGames.at(i).observerArray[k]->getSock(), "thegamehasnowended");
                     activeGames.at(i).observerArray[k] = NULL;
                     activeGames.at(i).observercount = 0;
                     return;
                }
                send.append(gamestate);
                sendData(activeGames.at(i).observerArray[k]->getSock(), send);
            }
    }
}

void processMove(int sock, string data) {
    int col = data[0] - '0';
    int row = data[2] - '0';

    User* currentUser = activeUsers[sock];
    Game* game = currentUser->getPlayer()->getGame();
    Player *opponent = game->getOpponent(currentUser->getPlayer());
    if (currentUser->getPlayer()->play(col, row)) { //valid move
        
        if (game->checkWin(currentUser->getPlayer())) {
            sendData(sock, "WIN");                                          //Sending the winner a "WIN" Statement
            //currentUser->getPlayer()->setWins();                                          //Will increment the Wins of the current User in the server
            
            //currentUser.setWins(getWins() + 1);
            

            sendData(opponent->getUser()->getSock(), "LOSS");
            //opponent->getPlayer()->setLoses();                              //Will increment the losers overall loses by one
            sendData(opponent->getUser()->getSock(), game->sendState());
            //currentUser.setLoses(getLoses() + 1);
           
            observe(game->getgameNum(), game->sendState(), 'F');

            setWinsandLoses(currentUser->returnUsername(), opponent->getUser()->returnUsername());
            return;

        }
        sendData(sock, "MOVE SUCCESS");
        Player *opponent = currentUser->getPlayer()->getGame()->getOpponent(currentUser->getPlayer());
        sendData(opponent->getUser()->getSock(), opponent->getGame()->sendState());
        //cout << "TESTING: " <<currentUser->getPlayer()->userName;
        //cout << "TESTING: "<< opponent->userName;
        observe(game->getgameNum(), game->sendState(), currentUser->getPlayer()->getMark());



    } else {
        sendData(sock, "MOVE FAILED");
    }

}

void putUserInGame(int sock) {
    User* user = activeUsers[sock];

    cout << "found sockets user " << user->getUserName() << endl;
    Player* player = new Player(user->getUserName());   //TODO this probably causes a memory leak
    user->setPlayer(player);

//    if (!potentialGame->isFull()) {
//        cout << "Found game with a spot" << endl;
//        potentialGame->setPlayer(player);
//    } else {
//        cout << "Made new game" << endl;
//        Game newGame = Game();
//        newGame.setPlayer(player);
//        activeGames.push_back(newGame);
//
//        potentialGame = &newGame;
//    }
    Game& potentialGame = activeGames[0];
    int i = 1;
    while (potentialGame.isFull()) {
        potentialGame = activeGames[i];
        i++;
    }

    potentialGame.setPlayer(player);

    if (potentialGame.isFull()) {
        sendData(potentialGame.getOPlayer()->getUser()->getSock(), "WAIT");

        sendData(potentialGame.getXPlayer()->getUser()->getSock(), potentialGame.sendState());
    }
}

void sendData(int sock, string data) {

    cout << "Sending to client: " << data << endl;

    const char *toSend = data.c_str();
    int bytesSent = 0;
    int totalBytesToSend = data.length();
    int messageLength = 0;
    int returnedFromSend = 0;

    while (totalBytesToSend != 0) {
        if (totalBytesToSend < BUFFERSIZE) {
            messageLength = totalBytesToSend;
        } else {
            messageLength = BUFFERSIZE;
        }

        returnedFromSend = send(sock, &toSend[bytesSent], messageLength, 0);
        bytesSent += returnedFromSend;

        totalBytesToSend -= returnedFromSend;
    }

}


void loginUser(int sock, string userName) {
    bool userLoggedIn = false;
    for (int i = 0; i <= registeredUsers.size(); i++) {
        cout << registeredUsers[i].getBannedStatus();
        if ((registeredUsers[i].getUserName() == userName) && registeredUsers[i].attemptLogin()) {
            if(registeredUsers[i].getBannedStatus()){
                sendData(sock, "LOGIN FAILED");
                break;
            }
            else{
                sendData(sock, "LOGIN SUCCESS");
                sendData(sock, listOfRunningGames());                    //Where initial room data is stored.
                userLoggedIn = true;
                registeredUsers[i].setOnlineStatus(true);
                registeredUsers[i].setSock(sock);
                activeUsers[sock] = &registeredUsers[i];
                break;
            }
        }
    }
    if (!userLoggedIn) {
        sendData(sock, "LOGIN FAILED");
    }
}

void displayLeaderboard(int sock) {
    
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
