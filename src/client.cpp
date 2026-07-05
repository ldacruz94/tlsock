
#include "client.h"
#include <netinet/in.h>
#include <cstring>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

TcpClient::TcpClient(std::string host, int port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ptonResult = inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (ptonResult != 1) {
        throw std::system_error(errno, std::generic_category(), "inet_pton() failed");
    }

    int result = connect(this->socket.getFd(), (struct sockaddr*)&addr, sizeof(addr));
    if (result == -1) {
        throw std::system_error(errno, std::generic_category(), "connect() failed");
    }
}

void TcpClient::send(const std::string& data) {
    ssize_t bytesSent = write(this->socket.getFd(), data.c_str(), data.size());

    if (bytesSent == -1) {
        throw std::system_error(errno, std::generic_category(), "error sending bytes");
    }
}

std::string TcpClient::receive() {
    char buffer[4096];
    ssize_t bytesRead = read(this->socket.getFd(), buffer, sizeof(buffer));

    if (bytesRead == -1) {
        throw std::system_error(errno, std::generic_category(), "error receiving bytes");
    }

    return std::string(buffer, bytesRead);
}