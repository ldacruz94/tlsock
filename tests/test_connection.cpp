#include <gtest/gtest.h>
#include "server.h"
#include "context.h"
#include "tls_connection.h"
#include <thread>
#include <chrono>

namespace {
    const int kTestPort = 5443;
    const std::string kCertPath = "../certs/cert.pem";
    const std::string kKeyPath = "../certs/key.pem";
    const std::string kUntrustedCertPath = "../certs/other_cert.pem";
    const int kUnusedPort = 5444;
    const std::string host = "127.0.0.1";
}

void runServerOnce() {
    try {
        TcpServer server(kTestPort);
        TlsContext serverCtx = TlsContext::forServer(kCertPath, kKeyPath);
        TlsConnection conn = TlsConnection::acceptTls(server, serverCtx);

        std::string received = conn.receive();
        conn.send("pong");
    } catch (const std::exception& e) {
        std::cerr << "runServerOnce: " << e.what() << std::endl;   
    }
}

TEST(TlsConnectionTest, ClientServerRoundTrip) {
    std::thread serverThread(runServerOnce);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    TlsContext clientCtx = TlsContext::forClient(kCertPath);
    TlsConnection conn = TlsConnection::connectTls(host, kTestPort, clientCtx);

    conn.send("ping");
    std::string response = conn.receive();

    EXPECT_EQ(response, "pong");

    serverThread.join();
}

TEST(TlsConnectionTest, HostnameMismatchFails) {
    std::thread serverThread(runServerOnce);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    TlsContext clientCtx = TlsContext::forClient(kCertPath);

    EXPECT_THROW(
        TlsConnection::connectTls("127.0.0.2", kTestPort, clientCtx),
        std::runtime_error
    );

    serverThread.join();
}

TEST(TlsConnectionTest, UntrustedCertFails) {
    std::thread serverThread(runServerOnce);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    TlsContext clientCtx = TlsContext::forClient(kUntrustedCertPath);

    EXPECT_THROW(
        TlsConnection::connectTls(host, kTestPort, clientCtx),
        std::runtime_error
    );

    serverThread.join();
}

TEST(TlsConnectionTest, ConnectionRefusedFails) {
    TlsContext clientCtx = TlsContext::forClient(kCertPath);

    EXPECT_THROW(
        TlsConnection::connectTls(host, kUnusedPort, clientCtx),
        std::system_error
    );
}