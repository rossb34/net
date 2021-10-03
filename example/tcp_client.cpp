#include "net/tcp_client.hpp"
#include <array>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./tcp_client host port" << std::endl;
        std::cout << "\thost: host of the endpoint to connect to, e.g. localhost" << std::endl;
        std::cout << "\tport: port of the endpoint to connect to, e.g. 9876" << std::endl;
        exit(EXIT_FAILURE);
    }
    // Instantiate a stream socket with default properties and options
    auto socket = std::make_unique<net::StreamSocket>();

    // Create a TCP client composed of the stream socket
    net::TcpClient client(std::move(socket));

    // Connect the TCP client to host:port
    const auto host = argv[1];
    const auto port = argv[2];
    std::cout << "Connecting to " << host << ":" << port << std::endl;
    try {
        client.connect(host, port);
    } catch (const net::IOError &ioError) {
        std::cerr << "Failed to connect: " << ioError.what() << std::endl;
    }

    // Send a message to the server
    const std::string msg = "hello\n";
    if (client.sendBytes(msg.c_str(), msg.length()) < msg.length()) {
        std::cerr << "Failed to send complete message to server" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read a message from the server
    std::array<char, 64> buffer{};
    const auto bytesRead = client.recvBytes(buffer.data(), buffer.size());
    if (bytesRead > 0) {
        std::cout << "Received message from server... ";
        std::cout << std::string_view(buffer.data(), bytesRead) << std::endl;
    }
}
