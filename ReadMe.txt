CPSC441 - ASSIGNMENT 3- GROUP COMPONENT- TIC_TAC_TOE Game
2019-11-08


To set up this networking game in RAC, all files to an instance and compile the server application via:  g++ -o Server Game.cpp Player.cpp SelectServer.cpp User.cpp
For all clients, upload all files to the RAC cloud and compile the client program like so: g++ -o Client TCPClient.cpp

The IP required to connect to the server process is the local IPv4 for the server instance. The listening port the server will use must be enter as ./Server <port>. 
After doing so the server will be initilized.

The clients must know the server IP as well as it's listening port and must connect via: ./Client <server IP> <listening port>

Afterwards, the console will provide information on how to run and control the game itself. 


Students:
Name : Gregory Ord                   
 
Name: August Sosick

Name: Nathan Darby

Name: Grigory Devyatov
