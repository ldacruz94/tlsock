
#pragma once

#include "socket.h"
#include "connection.h"

class TcpServer {

    public:
        TcpServer(int port);
        TcpConnection accept();

    private:
        Socket socket;
};