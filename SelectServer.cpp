/*
 * A simple TCP select server that accepts multiple connections and echo message back to the clients
 * For use in CPSC 441 lectures
 * Instructor: Prof. Mea Wang
 */

#include <iostream>
#include <sys/socket.h> // for socket(), connect(), send(), and recv()
#include <arpa/inet.h>  // for sockaddr_in and inet_addr()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()
#include <string>

#include "Tic_Tac_Toe.h"

using namespace std;

const int BUFFERSIZE = 32;   // Size the message buffers
const int MAXPENDING = 10;    // Maximum pending connections

fd_set recvSockSet;   // The set of descriptors for incoming connections
int maxDesc = 0;      // The max descriptor
bool terminated = false;

void initServer(int &, int &, int port);

void processSockets(fd_set);

void sendData(int, char[], int);

void receiveData(int, char[], int &);

int xPlayerSock;
int oPlayerSock;

static bool xPlayerSet = false;
static bool oPlayerSet = false;

static bool xPlayerTurn = true;

TicTacToe theGame = TicTacToe();

// string list();
// string get(string filename);

int main(int argc, char *argv[]) {

    int TCPSock;                  // server socket descriptor
    int UDPSock;
    int clientSock;                  // client socket descriptor
    struct sockaddr_in clientAddr;   // address of the client

    struct timeval timeout = {0, 10};  // The timeout value for select()
    struct timeval selectTime;
    fd_set tempRecvSockSet;            // Temp. receive socket set for select()

    // Check for input errors
    if (argc != 2) {
        cout << "Usage: " << argv[1] << " <Listening Port>" << endl;
        exit(1);
    }

    // Initilize the server
    initServer(TCPSock, UDPSock, atoi(argv[1]));
    cout << "\nServer init successful" << endl;

    // Clear the socket sets    
    FD_ZERO(&recvSockSet);

    // Add the listening socket to the set
    FD_SET(TCPSock, &recvSockSet);
    FD_SET(UDPSock, &recvSockSet);
    maxDesc = max(TCPSock, UDPSock);

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

            //sets the IP address to either the xPlayer or the oPlayer
            if (!xPlayerSet) {
                xPlayerSock = clientSock;
                xPlayerSet = true;
            } else if (xPlayerSet && !oPlayerSet) {
                oPlayerSock = clientSock;
                oPlayerSet = true;
                theGame.setup("xPlayer", "oPlayer"); //FIX THIS add usernames later
            } else {
                cout << "Player is not able to be added to the game." << endl;
            }



// PLAYERS ASSIGNED HERE



            // Add the new connection to the receive socket set
            FD_SET(clientSock, &recvSockSet);
            maxDesc = max(maxDesc, clientSock);
        } else if (FD_ISSET (UDPSock, &tempRecvSockSet)) {
            unsigned int size = sizeof(clientAddr);
            char inBuffer[BUFFERSIZE];
            // Clear the buffers
            memset(&inBuffer, 0, BUFFERSIZE);

            // set the size of the client address structure
            size = sizeof(clientAddr);

            // Receive the message from client
            int bytesRecv = recvfrom(UDPSock, (char *) &inBuffer, BUFFERSIZE, 0, (struct sockaddr *) &clientAddr,
                                     (socklen_t *) &size);

            // Check for errors   
            if (bytesRecv < 0) {
                cout << "recvfrom() failed, or the connection is closed. " << endl;
                exit(1);
            }

            cout << "Client " << inet_ntoa(clientAddr.sin_addr) << ":" << clientAddr.sin_port << ": " << inBuffer;

            // Echo the message back to the client
            int bytesSent = sendto(UDPSock, (char *) &inBuffer, bytesRecv, 0, (struct sockaddr *) &clientAddr,
                                   sizeof(clientAddr));

            // Check for errors
            if (bytesSent != bytesRecv) {
                cout << "error in sending" << endl;
                exit(1);
            }

        }
            // Then process messages waiting at each ready socket
        else
            processSockets(tempRecvSockSet);
    }
    // Close the connections with the client
    for (int sock = 0; sock <= maxDesc; sock++) {
        if (FD_ISSET(sock, &recvSockSet))
            close(sock);
    }

    // Close the server sockets
    close(TCPSock);
    close(UDPSock);

}

void initServer(int &TCPSock, int &UDPSock, int port) {
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

    // Create a UDP socket
    // * AF_INET: using address family "Internet Protocol address"
    // * SOCK_DGRAM: supports datagrams (connectionless, unreliable messages of a fixed (typically small) maximum length)
    // * IPPROTO_UDP: UDP protocol
    if ((UDPSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        cout << "socket() failed" << endl;
        exit(1);
    }

    // Bind to the local address
    if (bind(UDPSock, (sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        cout << "bind() failed" << endl;
        exit(1);
    }
}

void processSockets(fd_set readySocks) {
    char *buffer = new char[BUFFERSIZE];       // Buffer for the message from the server
    int size;                                    // Actual size of the message 

    // Loop through the descriptors and process
    for (int sock = 0; sock <= maxDesc; sock++) {
        if (!FD_ISSET(sock, &readySocks))
            continue;

        // Clear the buffers
        memset(buffer, 0, BUFFERSIZE);

        // Receive data from the client
        receiveData(sock, buffer, size);

        // Echo the message back to the client
        sendData(sock, buffer, size);
    }

    delete[] buffer;
}

void receiveData(int sock, char *inBuffer, int &size) {
    // Receive the message from client
    size = recv(sock, (char *) inBuffer, BUFFERSIZE, 0);

    // Check for connection close (0) or errors (< 0)
    if (size <= 0) {
        cout << "recv() failed, or the connection is closed. " << endl;
        FD_CLR(sock, &recvSockSet);

        // Update the max descriptor
        while (FD_ISSET(maxDesc, &recvSockSet) == false)
            maxDesc -= 1;
        return;
    }

    string msg = string(inBuffer);
    cout << "Client: " << msg;
}


void sendData(int sock, char *buffer, int size) {

    string sendToPlayer;
    int col = buffer[0] - '0';
    int row = buffer[2] - '0';
    if ((sock == xPlayerSock) && xPlayerTurn) {
        //make a move with xplayer
        sendToPlayer = theGame.makeMove(xPlayerTurn, col, row);
        xPlayerTurn = false;
    } else if ((sock == oPlayerSock) && (!xPlayerTurn)) {
        //make a move with the oPlayer
        sendToPlayer = theGame.makeMove(xPlayerTurn, col, row);
        xPlayerTurn = true;
    } else if ((sock == xPlayerSock) && !xPlayerTurn) {
        sendToPlayer = "Error: Waiting for opponent to make a move...";
    } else if ((sock == oPlayerSock) && xPlayerTurn) {
        sendToPlayer = "Error: Waiting for opponent to make a move...";
    }

    send(sock, sendToPlayer.c_str(), sendToPlayer.length(), 0);
    // send(xPlayerSock, sendToPlayer.c_str(), sendToPlayer.length(), 0);
    // send(oPlayerSock, sendToPlayer.c_str(), sendToPlayer.length(), 0);


    return;
}

// void sendData (int sock, char* buffer, int size)
// {
//     int bytesSent = 0;                   // Number of bytes sent

// Sent the data
// bytesSent += send(sock, (char *) buffer + bytesSent, size - bytesSent, 0);

// if(strncmp(buffer, (char*)"list", 4) == 0){
//     string listFiles = list();
//     size = listFiles.length();
//     bytesSent += send(sock, listFiles.c_str() + bytesSent, size - bytesSent, 0);
// }else if(strncmp(buffer, (char*)"get", 3) == 0){
//     string filename;
//     int bufferLength = sizeof(buffer)/sizeof(char);
//     for(int i = 4; i < bufferLength; i++){
//         filename += buffer[i];
//     }
//     get(filename);
// }else if (strncmp(buffer, "logout", 6) == 0){
//     bytesSent = 1;
//     size = 1;
//     return;
// }else if (strncmp(buffer, "terminate", 9) == 0){
//     terminated = true;
//     return;
// }else{
//     string cmd = string(buffer);
//     string returnMsg = "Unknown Command: " + cmd + '\n';
//     delete[] buffer;
//     buffer = new char[returnMsg.length() + 1];
//     // buffer = new char[BUFFERSIZE];
//     strcpy(buffer, returnMsg.c_str());
//     size = returnMsg.length();
//     // int n = size/BUFFERSIZE;
//     // for(int i = 0; i <= n; i++){
//     //     bytesSent += send(sock, (char *) buffer + bytesSent, size - bytesSent, 0);
//     // }
//     bytesSent += send(sock, (char *) buffer + bytesSent, size - bytesSent, 0);
// }

// if (bytesSent < 0 || bytesSent != size)
// {
//     cout << "error in sending" << endl;
//     return;
// }

// if (strncmp(buffer, "terminate", 9) == 0)
//     terminated = true;
// }

// string list(){
//     // Execute the "ls" command and save the output to /tmp/temp.txt.
//     // /tmp is a special directory storing temporate files in Linux.
//     system("ls > /tmp/temp.txt");

//     // Open the file
//     ifstream infile;
//     infile.open("/tmp/temp.txt");

//     // Store the file content into a string
//     string line;
//     string data = "";
//     while (getline(infile, line))
//     {
//         data += line + "\n";
//     }

//     // Close the file
//     infile.close();

//     return data;
// }

// string get(string filename){
//     // Open the file for input
//     // cout << "Open file: " << filename << endl;
//     ifstream infile;
//     infile.open(filename.c_str());

//     // check for errors in opening the file
//     if (!infile.is_open())
//     {
//         cout << "open() failed " << endl;
//         string msg = "Error in openning file " + filename + "\n";
//         cout << msg;
//         return msg;
//     }

//     // Read the file line by line
//     string data;
//     string line;
//     while (getline(infile, line))
//     {
//         line += "\n";
//         data += line;
//     }

//     return data;
// }


    