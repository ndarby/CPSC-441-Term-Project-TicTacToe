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

using namespace std;

const int BUFFERSIZE = 32;   // Size the message buffers

int initConnection(const char *, int);

string receiveData(int);

void sendData(int, string);

string getValidInput();

int main(int argc, char *argv[]) {
    int sock;                        // A socket descriptor

    // Check for input errors
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <Server IP> <Server Port>" << endl;
        exit(1);
    }

    sock = initConnection(argv[1], atoi(argv[2]));

    string userInput = " ";
    string dataFromServer;
    while (userInput != "logout") {

        userInput = getValidInput();

        cout << "userInput: " << userInput << endl;
        sendData(sock, userInput);

        dataFromServer = receiveData(sock);
        
    }

    // Close the socket
    close(sock);
    exit(0);
}

string receiveData(int sock) {

    char inBuffer[BUFFERSIZE];
    string dataReceived = "";
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
        
        dataReceived.pop_back(); //to get rid of \003 that is attached by send()
        dataReceived.pop_back();
        dataReceived.pop_back();
        dataReceived.pop_back();

		        
        


        memset(&inBuffer, 0, BUFFERSIZE);
    } while (bytesRecv == BUFFERSIZE);
    
    cout << "Server: \n" << dataReceived << endl;

    return dataReceived;
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

string getValidInput() {
    string returnValue = "   ";

    while (true) {
        char input[BUFFERSIZE];
        fgets(input, BUFFERSIZE, stdin);

        if (strncmp(input, "logout", 6) == 0) {
            return "logout";
        } else if (strncmp(input, "leaderboard", strlen("leaderboard")) == 0) {
            return "leaderboard";
        } else {
            int row = -1, col = -1;
            sscanf(input, "%d %d", &row, &col);
            cout << "row: " << row << " col: " << col << endl;
            if ((row >= 0) && (row <= 2)) {
                if ((col <= 2) && (col >= 0)) {
                    memset(&input, 0, BUFFERSIZE);

                    input[0] = row  + '0';
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