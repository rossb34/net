#ifndef NET_ADDRESS_RESOLVER_HPP
#define NET_ADDRESS_RESOLVER_HPP

#include <cstring>
#include <exception>
#include <netdb.h>
#include <string>

namespace net {

    /// AddressInfoError Exception
    class AddressInfoError : std::exception {
    public:
        explicit AddressInfoError(const std::string &msg) : msg_(msg) {}

        [[nodiscard]] const char *what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
    };


    /// Network Address Translation
    class AddressResolver {
    public:
        /// Default constructs an instance of an `AddressResolver`
        AddressResolver() : addrInfo_(nullptr) {}

        virtual ~AddressResolver() {
            if (addrInfo_)
                freeaddrinfo(addrInfo_);
        }

        /// Resolves the host and port to a set of socket addresses
        ///
        /// \param host hostname of the endpoint
        /// \param port port of the endpoint
        /// \return returns a linked list of addrinfo structures. On error, throws an exception of type
        /// `AddressInfoError` with string describing the meaning of the error.
        struct addrinfo *resolve(const std::string &host, const std::string &port) {
            // The functionality to connect a socket to an endpoint is referenced from
            // https://man7.org/linux/man-pages/man3/getaddrinfo.3.html

            // Initialize hints for TCP socket (SOCK_STREAM) and IPv4 address (AF_INET)
            struct addrinfo hints{};
            memset(&hints, 0, sizeof hints);

            // Specify AF_UNSPEC for any address family, e.g. IPv4 or IPv6
            hints.ai_family = AF_UNSPEC;

            // Specify 0 for any socket type, e.g. SOCK_STREAM or SOCK_DGRAM
            hints.ai_socktype = 0;

            // Allocates and initializes a linked list of addrinfo structures pointed to by addrInfo_
            auto result = getaddrinfo(host.c_str(), port.c_str(), &hints, &addrInfo_);
            if (result < 0) {
                throw AddressInfoError(gai_strerror(result));
            }
            return addrInfo_;
        }

    private:
        struct addrinfo *addrInfo_;
    };

}

#endif //NET_ADDRESS_RESOLVER_HPP
