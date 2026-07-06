
#include "tls_connection.h"
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>
#include <system_error>

TlsConnection::TlsConnection(Socket socket, SSL* ssl) : socket(std::move(socket)), ssl(ssl) {}

TlsConnection::~TlsConnection() {
    if (this->ssl != nullptr) {
        SSL_free(this->ssl);
    }
}

TlsConnection::TlsConnection(TlsConnection&& other) noexcept {
    if (this != &other) {
        this->socket = std::move(other.socket);
        this->ssl = other.ssl;
        other.ssl = nullptr;   
    }
}

TlsConnection& TlsConnection::operator=(TlsConnection&& other) noexcept {
    if (this != &other) {
        if (this->ssl != nullptr) {
            SSL_free(this->ssl);
        }
        this->ssl = other.ssl;
        other.ssl = nullptr;
        this->socket = std::move(other.socket);
    }

    return *this;
}

void TlsConnection::send(const std::string &data) {
    int result = SSL_write(this->ssl, data.c_str(), data.size());

    if (result <= 0) {
        throw std::runtime_error("SSL_write failed"); 
    }
}

std::string TlsConnection::receive() {
    char buffer[4096];
    int bytesRead = SSL_read(this->ssl, buffer, sizeof(buffer));

    if (bytesRead <= 0) {
        throw std::runtime_error("SSL_read failed");
    }

    return std::string(buffer, bytesRead);
}

TlsConnection TlsConnection::connectTls(const std::string& host, int port, const TlsContext& context) {
    Socket socket;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int ptonResult = inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

    if (ptonResult != 1) {
        throw std::system_error(errno, std::generic_category(), "inet_pton() failed");
    }

    int result = ::connect(socket.getFd(), (struct sockaddr*)&addr, sizeof(addr));
    if (result == -1) {
        throw std::system_error(errno, std::generic_category(), "connect() failed");
    }

    SSL* ssl = SSL_new(context.getRaw());
    if (ssl == nullptr) {
        throw std::runtime_error("SSL_new failed");
    }

    int setFdResult = SSL_set_fd(ssl, socket.getFd());
    if (setFdResult != 1) {
        SSL_free(ssl);
        throw std::runtime_error("Setting SSL on FD failed");
    }

    int connectionResult = SSL_connect(ssl);
    if (connectionResult != 1) {
        SSL_free(ssl);
        throw std::runtime_error("SSL connection failed!");
    }

    return TlsConnection(std::move(socket), ssl);
}

TlsConnection TlsConnection::acceptTls(TcpServer& server, const TlsContext& context) {
    Socket socket = server.acceptSocket();
    SSL* ssl = SSL_new(context.getRaw());

    if (ssl == nullptr) {
        throw std::runtime_error("SSL creation failed");
    }

    int setFd = SSL_set_fd(ssl, socket.getFd());
    if (setFd != 1) {
        SSL_free(ssl);
        throw std::runtime_error("Setting SSL on FD failed");
    }

    int acceptResult = SSL_accept(ssl);
    if (acceptResult != 1) {
        SSL_free(ssl);
        throw std::runtime_error("Accepting SSL failed");
    }

    return TlsConnection(std::move(socket), ssl);
}