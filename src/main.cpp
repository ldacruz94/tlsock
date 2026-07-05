
#include <iostream>
#include "server.h"
#include "connection.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "usage: program server <port> or program client <host> <port>" << std::endl;
        return 1;
    }

    std::string mode = argv[1]; 

    try {
        if (mode == "server") {
            int port = std::stoi(argv[2]);
            TcpServer server = TcpServer(port);
            std::cout << "Server started on port: " << port << std::endl;

            TcpConnection conn = server.accept();
            std::string result = conn.receive();
            std::cout << result << std::endl;
            conn.send("Hello from server!");
        } else if (mode == "client") {
            std::string host = argv[2];
            int port = std::stoi(argv[3]);
            
            TcpConnection conn = TcpConnection::connect(host, port);
            conn.send("Hello from client!");
            std::string response = conn.receive();
            std::cout << response << std::endl;
        } else {
            std::cout << "Mode can either be `server` or `client`" << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
