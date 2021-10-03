#ifndef NET_TCP_CLIENT_HPP
#define NET_TCP_CLIENT_HPP

#include <memory>
#include <poll.h>
#include "socket.hpp"
#include "address_resolver.hpp"

namespace net {

    class TcpClient {
    public:
        /// Constructs a TCP Client compose of socket
        ///
        /// \param socket socket for the underlying connection
        explicit TcpClient(std::unique_ptr<StreamSocket> socket) : socket_(std::move(socket)) {}

        /// Gets the stream socket
        StreamSocket *socket() {
            return socket_.get();
        }

        /// Gets the stream socket
        [[nodiscard]] const StreamSocket *socket() const {
            return socket_.get();
        }

        /// Connects the socket
        ///
        /// \param host host name or IP address of the endpoint to connect to
        /// \param port port of the endpoint to connect to
        void connect(const std::string &host, const std::string &port) const {

            // Resolve the address to get address info used to connect
            AddressResolver resolver;
            // Let resolve throw here. If I can't resolve the address, then I can't connect.
            const auto *ai = resolver.resolve(host, port);

            if (socket_->getBlocking()) {
                connectBlocking(ai);
            } else {
                connectNonBlocking(ai);
            }
        }

        /// Sends bytes to the stream socket
        ///
        /// \param buffer buffer of data to send.
        /// \param len number of bytes to send.
        ssize_t sendBytes(const char *buffer, size_t len) const {
            return socket_->sendBytes(buffer, len);
        }

        /// Receives bytes from the stream socket
        ///
        /// \param buffer buffer to receive len bytes of data.
        /// \param len maximum number of bytes to read (i.e. length of buffer).
        ssize_t recvBytes(char *buffer, size_t len) const {
            return socket_->recvBytes(buffer, len);
        }

    private:
        std::unique_ptr<StreamSocket> socket_;

        /// Simple connect logic for a blocking socket
        void connectBlocking(const struct addrinfo *ai) const {
            bool connected = false;
            for (const auto *p = ai; p != nullptr; p = p->ai_next) {
                if (::connect(socket_->fd(), p->ai_addr, p->ai_addrlen) == 0) {
                    connected = true;
                    break;
                }
            }

            // The connection failed, throw an exception
            if (!connected) {
                throw IOError(errno, strerror(errno));
            }
        }

        /// Connect logic for a non-blocking socket
        void connectNonBlocking(const struct addrinfo *ai) const {
            // structure to poll the socket file descriptor for readiness to write
            struct pollfd pfd{};
            pfd.fd = socket_->fd();
            pfd.events = POLLOUT;
            const auto pollTimeoutMillis = 5000;
            int socketErrorCode = 0;

            for (const auto *p = ai; p != nullptr; p = p->ai_next) {
                if (::connect(socket_->fd(), p->ai_addr, p->ai_addrlen) == 0) {
                    return;
                }

                if (errno == EINPROGRESS) {
                    // connect() returned a non zero value and errno indicates the connect operation is in progress

                    // From connect() docs... The socket is nonblocking and the connection cannot be completed
                    // immediately. It is possible to select(2) or poll(2) for completion by selecting the socket
                    // for writing. After select(2) indicates writability, use getsockopt(2) to read the SO_ERROR
                    // option at level SOL_SOCKET to determine whether connect() completed successfully (SO_ERROR is
                    // zero) or unsuccessfully (SO_ERROR is one of the usual error codes listed here, explaining the
                    // reason for the failure).
                    const auto pollResult = poll(&pfd, 1, pollTimeoutMillis);
                    if (pollResult == 1) {
                        socklen_t len = sizeof(int);
                        if (socket_->getOptionRaw(SOL_SOCKET, SO_ERROR, &socketErrorCode, &len) and socketErrorCode == 0) {
                            return;
                        }
                    } else if (pollResult == -1) {
                        throw IOError(errno, strerror(errno));
                    }
                }
            }

            // If we get to this point, then we have exhausted all addresses and have failed to connect.
            // Throw an IOError with the socket error code from getsockopt(2) to read the SO_ERROR option
            throw IOError(socketErrorCode, strerror(socketErrorCode));
        }
    };

}

#endif //NET_TCP_CLIENT_HPP
