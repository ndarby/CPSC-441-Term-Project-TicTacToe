cmake_minimum_required(VERSION 3.10)

project(CPSC-441-Term-Project-TicTacToe)

set(CMAKE_CXX_STANDARD 11)

add_executable(TCPClient
        TCPClient.cpp)

add_executable(SelectServer
        SelectServer.cpp
        Tic_Tac_Toe.cpp Tic_Tac_Toe.h
        Player.h
        constants.h)