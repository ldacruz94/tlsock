
#pragma once

#include "socket.h"
#include "tcp_connection.h"

class TcpServer {

    public:
        TcpServer(int port);
        TcpConnection accept();
        Socket acceptSocket();

    private:
        Socket socket;
};