/*
 * A simple TCP client that sends messages to a server and display the message
   from the server.
 * Modified for use in CPSC 441 Term Project
 */

#include <iostream>
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()
#include <string>

using namespace std;

bool adminAccount = false;

const int BUFFERSIZE = 32;   // Size the message buffers

void banUser(int sock);

void joinALiveRoom(int sock);

int initConnection(const char *, int);

void deleteRoom(int sock);

string receiveData(int);

void sendData(int, string);

string getValidInput();

void attemptLogin(int sock);

void menuOptions(int sock);

void enterGame(int sock);

void createNewRoom(int sock);

int main(int argc, char *argv[]) {
    int sock;                        // A socket descriptor

    // Check for input errors
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <Server IP> <Server Port>" << endl;
        exit(1);
    }

    sock = initConnection(argv[1], atoi(argv[2]));

    attemptLogin(sock);

    menuOptions(sock);

    // Close the socket
    close(sock);
    exit(0);
}

void menuOptions(int sock) {
    if(adminAccount == true){                   //admin account
        cout << "You are currently running as an administrator.\n";
        cout << "The menu options are: 'logout', 'leaderboard', 'KILLSERVER', 'listusers',\n'createroom', 'joinroom', 'deleteroom', 'banUser'" << endl;
    }
    else{                                       //regular account
        cout << "The menu options are: 'logout', 'leaderboard', 'KILLSERVER', 'listusers',\n'createroom', 'joinroom', 'deleteroom'" << endl;
    }
    

    string userInput;
    while (true) {
        cin >> userInput;
        if (userInput == "logout") {
            //TODO nicely logout
            break;
        } else if (userInput == "leaderboard") {
            sendData(sock, "LEADERBOARD");
            cout<< receiveData(sock);
        } else if (userInput == "play") {
            sendData(sock, "PLAY");
            enterGame(sock);
        } else if (userInput == "KILLSERVER"){
            sendData(sock,"KILLSERVER");
        }else if (userInput == "listusers"){
            sendData(sock,"LISTUSERS");
            cout<< receiveData(sock);
        }else if (userInput == "createroom"){
            createNewRoom(sock);
        }else if (userInput == "joinroom"){
            //sendData(sock,"JOINROOM");
            //cout<< receiveData(sock);
            joinALiveRoom(sock);

        }else if (userInput == "deleteroom"){
            deleteRoom(sock);
        }else if (userInput == "banUser"){
            banUser(sock);
        }
    }
}

void banUser(int sock){
    string userToBan;
    sendData(sock,"BANUSER");
    cout<< "Please give the name of the User you'd like to ban\n";
    cin >> userToBan;
    sendData(sock,userToBan);
    cout << receiveData(sock);
}

void joinALiveRoom(int sock){
    string joinNum;
    sendData(sock,"JOINROOM");
    cout<< "Please give the number of the room you'd like to join";
    cin >> joinNum;
    sendData(sock,joinNum);

    string userInput = " ";
    string dataFromServer;
    bool endGame = false;

    cout << "Start of game" << endl;

    dataFromServer = receiveData(sock);
    if (dataFromServer.find("WAIT") == 0) {
        cout << "Opponent goes first" << endl;
        dataFromServer = receiveData(sock);
    }
    while (!endGame) {

        cout << dataFromServer;

        userInput = getValidInput();
        sendData(sock, userInput);

        dataFromServer = receiveData(sock);

        if (dataFromServer == "MOVE SUCCESS") {
            cout << "Nice move. Wait for opponent" << endl;
            dataFromServer = receiveData(sock);

            if (dataFromServer.find("LOSS") == 0) {
                cout << "You lost" << endl;
                dataFromServer = receiveData(sock);
                cout << dataFromServer;
                endGame = true;
            } else {
                cout << dataFromServer;
            }
        } else if (dataFromServer == "MOVE FAILED") {
            cout << "Illegal move. Try again" << endl;
            continue;

        } else if (dataFromServer == "WIN") {
            cout << "You won!" << endl;
            endGame = true;
        }

    }
}

void deleteRoom(int sock){
    string removeNum;
    sendData(sock,"DELETEROOM");
    cout<< "Please give the room number";
    cin >> removeNum;
    sendData(sock,removeNum);
    cout<< removeNum;
    cout<<receiveData(sock);
}

void createNewRoom(int sock){
    string gameroomNumber;
    sendData(sock, "CREATEROOM");
    cout<< "Please give the room number";
    cin >> gameroomNumber;
    sendData(sock,gameroomNumber);
    cout<<receiveData(sock);
}

void enterGame(int sock) {
    string userInput = " ";
    string dataFromServer;
    bool endGame = false;

    cout << "Start of game" << endl;

    dataFromServer = receiveData(sock);
    if (dataFromServer.find("WAIT") == 0) {
        cout << "Opponent goes first" << endl;
        dataFromServer = receiveData(sock);
    }
    while (!endGame) {

        cout << dataFromServer;

        userInput = getValidInput();
        sendData(sock, userInput);

        dataFromServer = receiveData(sock);

        if (dataFromServer == "MOVE SUCCESS") {
            cout << "Nice move. Wait for opponent" << endl;
            dataFromServer = receiveData(sock);

            if (dataFromServer.find("LOSS") == 0) {
                cout << "You lost" << endl;
                dataFromServer = receiveData(sock);
                cout << dataFromServer;
                endGame = true;
            } else {
                cout << dataFromServer;
            }
        } else if (dataFromServer == "MOVE FAILED") {
            cout << "Illegal move. Try again" << endl;
            continue;

        } else if (dataFromServer == "WIN") {
            cout << "You won!" << endl;
            endGame = true;
        }

    }
}

string receiveData(int sock) {

    char inBuffer[BUFFERSIZE];
    string dataReceived;
    int bytesRecv = 0;
    int totalLength = 0;

    do {
        bytesRecv = recv(sock, (char *) &inBuffer, BUFFERSIZE, 0);

        if (bytesRecv <= 0) {
            cerr << "recv() failed, or the connection is closed. " << endl;
            exit(1);
        }

        dataReceived.append(inBuffer);

        totalLength += bytesRecv;
//
//        dataReceived.pop_back(); //to get rid of \003 that is attached by send()
//        dataReceived.pop_back();
//        dataReceived.pop_back();
//        dataReceived.pop_back();

       if (bytesRecv == BUFFERSIZE) {
           dataReceived.pop_back(); //to get rid of \003 that is attached by send()
           dataReceived.pop_back();
           dataReceived.pop_back();
           dataReceived.pop_back();
       }

        memset(&inBuffer, 0, BUFFERSIZE);

    } while (bytesRecv == BUFFERSIZE);

    return
            dataReceived;
}

void sendData(int sock, string data) {
    char outBuffer[BUFFERSIZE];

    strncpy(outBuffer, data.c_str(), BUFFERSIZE);
    outBuffer[data.length()] = '\0';

    cout << "sending: " << outBuffer << endl;

    int bytesSent = send(sock, (char *) &outBuffer, strlen(outBuffer), 0);

    if (bytesSent < 0 || bytesSent != strlen(outBuffer)) {
        cerr << "error in sending" << endl;
        exit(1);
    }
}

void attemptLogin(int sock) {
    cout << "Enter username: " << endl;

    string userName;
    string serverString;
    string serverResponse;

    while (true) {
        cin >> userName;

        serverString = "LOGIN " + userName;
        sendData(sock, serverString);
        serverResponse = receiveData(sock);

        if (serverResponse == "LOGIN SUCCESS") {
            cout << receiveData(sock);                      //Where initial room listing is displayed
            break;
        }
        cout << "Your username is either incorrect or you have been banned from this server\n" << endl;
   

    }

    if(userName == "Admin"){
        adminAccount = true;
    }
}

string getValidInput() {
    string returnValue = "   ";

    while (true) {
        char input[BUFFERSIZE];
        fgets(input, BUFFERSIZE, stdin);

        if (strncmp(input, "logout", 6) == 0) {
            return "logout";
        } else {
            int row = -1, col = -1;
            sscanf(input, "%d %d", &row, &col);
            //cout << "row: " << row << " col: " << col << endl;
            if ((row >= 0) && (row <= 2)) {
                if ((col <= 2) && (col >= 0)) {
                    memset(&input, 0, BUFFERSIZE);

                    input[0] = row + '0';
                    input[1] = ' ';
                    input[2] = col + '0';
                    input[3] = '\0';
                    return string(input);
                }
            }
        }
        cout << "invalid input" << endl;
    }
}

int initConnection(const char *ipAddress, int portNum) {

    struct sockaddr_in serverAddr;
    int sock;

    // Create a TCP socket
    // * AF_INET: using address family "Internet Protocol address"
    // * SOCK_STREAM: Provides sequenced, reliable, bidirectional, connection-mode byte streams.
    // * IPPROTO_TCP: TCP protocol
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
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
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        cout << "setsockopt() failed" << endl;
        exit(1);
    }

    // Initialize the server information
    // Note that we can't choose a port less than 1023 if we are not privileged users (root)
    memset(&serverAddr, 0, sizeof(serverAddr));         // Zero out the structure
    serverAddr.sin_family = AF_INET;                    // Use Internet address family
    serverAddr.sin_port = htons(portNum);         // Server port number
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress);    // Server IP address

    // Connect to the server
    // * sock: the socket for this connection
    // * serverAddr: the server address
    // * sizeof(*): the size of the server address
    if (connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        cout << "connect() failed" << endl;
        exit(1);
    }

    return sock;
}
