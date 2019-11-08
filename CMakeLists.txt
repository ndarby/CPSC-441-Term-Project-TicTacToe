cmake_minimum_required(VERSION 3.10)

project(CPSC-441-Term-Project-TicTacToe)

set(CMAKE_CXX_STANDARD 11)

add_executable(TCPClient
        TCPClient.cpp)

add_executable(SelectServer
        SelectServer.cpp
        User.cpp User.h
        Game.cpp Game.h
        Board.h
        Player.cpp Player.h
        constants.h)