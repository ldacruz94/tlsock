cmake_minimum_required(VERSION 3.10)
project(tlsock CXX)

find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tlsock 
    ./src/main.cpp
    ./src/socket.cpp
    ./src/server.cpp
    ./src/context.cpp
    ./src/tls_connection.cpp
    ./src/tcp_connection.cpp
)

target_link_libraries(tlsock 
OpenSSL::SSL
OpenSSL::Crypto)