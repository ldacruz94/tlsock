
#include "server.h"
#include <netinet/in.h>
#include <cstring>
#include <system_error>
#include <sys/socket.h>
#include <unistd.h>

TcpServer::TcpServer(int port){
    int optVal = 1;
    setsockopt(this->socket.getFd(), SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)); // basically, reuse the same port even if it's cooling down from the last restart

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY; // bind to ALL available network interfaces

    int bindResult = bind(this->socket.getFd(), (struct sockaddr*)&addr, sizeof(addr));
    if (bindResult == -1) {
        throw std::system_error(errno, std::generic_category(), "bind() failed");
    }

    int totalPendingConnections = 150; // default amount of pending connections that server will listen to before rejecting new ones
    int listenResult = listen(this->socket.getFd(), totalPendingConnections);

    if (listenResult == -1) {
        throw std::system_error(errno, std::generic_category(), "listen() failed");
    }
}

TcpConnection TcpServer::accept() {
    return TcpConnection(this->acceptSocket());
}

Socket TcpServer::acceptSocket() {
    int connectionFd = ::accept(this->socket.getFd(), nullptr, nullptr); // might replace the nulls later if I want to capture client IPs
    
    if (connectionFd == -1) {
       throw std::system_error(errno, std::generic_category(), "accept() failed");
    }

    return Socket(connectionFd);
}
