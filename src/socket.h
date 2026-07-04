
#pragma once

/*
    Socket class that contains a file descriptor `fd` that the OS uses to identify that it's an open resource 
*/
class Socket {

public:
    Socket(int fd);
    Socket();
    Socket(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;
    Socket& operator=(const Socket&) = delete;

    ~Socket();
    int getFd() const;

private:
    int fd;
};
