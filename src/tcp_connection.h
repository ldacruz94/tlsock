
#pragma once

#include "socket.h"
#include <string>

class TcpConnection {
    public:
        TcpConnection(Socket socket);
        void send(const std::string& data);
        std::string receive();
        static TcpConnection connect(const std::string& host, int port);

    private:
        Socket socket;
};