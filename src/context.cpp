
#include "context.h"
#include <stdexcept>

TlsContext::TlsContext(SSL_CTX* ctx) {
    this->context = ctx;
}

TlsContext::~TlsContext() {
    if (this->context != nullptr) {
        SSL_CTX_free(this->context);
    }
}

TlsContext::TlsContext(TlsContext&& other) noexcept {
    this->context = other.context;
    other.context = nullptr;
}

TlsContext& TlsContext::operator=(TlsContext&& other) noexcept {
    if (this != &other) {
        if (this->context != nullptr) {
            SSL_CTX_free(this->context);
        }
        this->context = other.context;
        other.context = nullptr;
    }

    return *this;
}

SSL_CTX* TlsContext::getRaw() const {
    return this->context;
}

TlsContext TlsContext::forClient(const std::string& caCertPath) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == nullptr) {
        throw std::runtime_error("Failed to create new SSL failed");
    }

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, nullptr);
    int result = SSL_CTX_load_verify_locations(ctx, caCertPath.c_str(), nullptr);
    if (result != 1) {
        SSL_CTX_free(ctx);
        throw std::runtime_error("ca cert path verification failed");
    }

    return TlsContext(ctx);
}

TlsContext TlsContext::forServer(const std::string& certPath, const std::string& keyPath) {
    SSL_CTX* ctx = SSL_CTX_new(TLS_server_method());
    if (ctx == nullptr) {
        throw std::runtime_error("Failed to create new SSL context");
    }
    
    int useCertResult = SSL_CTX_use_certificate_file(ctx, certPath.c_str(), SSL_FILETYPE_PEM);    
    if (useCertResult != 1) {
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to use cert");
    }

    int useKeyResult = SSL_CTX_use_PrivateKey_file(ctx, keyPath.c_str(), SSL_FILETYPE_PEM);
    if (useKeyResult != 1) {
        SSL_CTX_free(ctx);
        throw std::runtime_error("Failed to use key");
    }
    
    return TlsContext(ctx);
}