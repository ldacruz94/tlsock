
#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>

/*
    This is the context we're using to setup the TLS behavior for either the client or server.
    The purpose here is to reuse the context to create the actual live SSL sessions that will be used in
    communication between the client and server.

    We're using a factory pattern here to generate the proper context depending on what's calling it (e.g., client or server).
*/
class TlsContext {

    public: 
        static TlsContext forClient(const std::string& caCertPath);
        static TlsContext forServer(const std::string& certPath, const std::string& keyPath);
        ~TlsContext();
        
        TlsContext(const TlsContext&) = delete;
        TlsContext(TlsContext&& other) noexcept;
        TlsContext& operator=(TlsContext&& other) noexcept;
        
        SSL_CTX* getRaw() const;

    private:
        SSL_CTX* context;
        TlsContext(SSL_CTX* ctx); // to enforce the factory pattern
};