
#pragma once
#include <string>
#include "socket.h"

class TcpClient {

    public:
        TcpClient(std::string host, int port);
        void send(const std::string& data);
        std::string receive();
        
    private:
        Socket socket;
};