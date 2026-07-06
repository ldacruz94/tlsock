
#pragma once

#include "socket.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include "context.h"
#include "server.h"

class TlsConnection {

    public: 
        void send(const std::string& data);
        std::string receive();
        TlsConnection(Socket socket, SSL* ssl);
        ~TlsConnection();
        TlsConnection(const TlsConnection&) = delete;
        TlsConnection(TlsConnection&& other) noexcept;
        TlsConnection& operator=(TlsConnection&& other) noexcept;        
        static TlsConnection connectTls(const std::string& host, int port, const TlsContext& context);
        static TlsConnection acceptTls(TcpServer& server, const TlsContext& context);

    private:
        Socket socket;
        SSL* ssl;
};