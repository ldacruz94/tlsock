
#include "socket.h"
#include <unistd.h>
#include <sys/socket.h>
#include <system_error>
#include <stdexcept>

Socket::Socket(int fd) {
    if (fd < 0) {
        throw std::invalid_argument("fd cannot be negative");
    }
    
    this->fd = fd;
}

Socket::Socket() {
    int defaultProtocol = 0; // defaults this to TCP

    int fd = socket(AF_INET, SOCK_STREAM, defaultProtocol);

    if (fd == -1) {
        throw std::system_error(errno, std::generic_category(), "socket() failed");
    }

    this->fd = fd;
}

Socket::Socket(Socket&& other) noexcept {
    this->fd = other.fd;
    other.fd = -1; // set the `other` object to sentinel so that there are not two objects with the same file descriptor
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (this->fd != -1) {
            close(this->fd);
        }
        this->fd = other.fd;
        other.fd = -1;
    }

    return *this;
}

Socket::~Socket() {
    if (this->fd != -1) {
        close(fd);
    };
}

int Socket::getFd() const {
    return this->fd; 
}