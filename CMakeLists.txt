cmake_minimum_required(VERSION 3.10)
project(tlsock CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tlsock 
    ./src/main.cpp
    ./src/socket.cpp
    ./src/server.cpp
    ./src/connection.cpp
)
