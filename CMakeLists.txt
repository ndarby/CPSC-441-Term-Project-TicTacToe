cmake_minimum_required(VERSION 3.10)

project(CPSC-441-Term-Project-TicTacToe)

set(CMAKE_CXX_STANDARD 11)

add_executable(TCPClient
        TCPClient.cpp)

add_executable(SelectServer
        SelectServer.cpp)

#add_executable(TicTacToe
#        Tic_Tac_Toe.cpp Tic_Tac_Toe.h
#        Game.h
#        Player.h
#        Referee.h
#        constants.h)