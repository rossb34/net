#ifndef NET_SOCKET_HPP
#define NET_SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

namespace net {

    /// Stream Socket Class
    ///
    /// Socket of type `SOCK_STREAM`
    class StreamSocket {
    public:
        /// Stream Socket Constructor
        ///
        /// Default behavior constructs a socket in blocking mode
        StreamSocket() : fd_(socket(AF_INET, SOCK_STREAM, 0)), blocking_(true) {}

        /// Stream Socket Constructor
        ///
        /// \param blocking blocking mode of the socket. If `true`, a blocking socket is constructed. If `false` a
        /// non-blocking is constructed. The non-blocking socket is constructed with mode
        /// `SOCK_STREAM | SOCK_NONBLOCK`.
        explicit StreamSocket(bool blocking)
                : fd_(socket(AF_INET, blocking ? SOCK_STREAM : SOCK_STREAM | SOCK_NONBLOCK, 0)), blocking_(blocking) {}

        /// Stream Socket Destructor
        ///
        /// Closes the socket file descriptor if valid
        virtual ~StreamSocket() {
            if (fd_ != -1) {
                close(fd_);
            }
        }

        /// Gets the file descriptor of the socket
        ///
        /// \return returns the file descriptor of the socket. A value of -1 indicates an invalid file descriptor.
        [[nodiscard]] int fd() const {
            return fd_;
        }

        /// Sets the raw option on the socket
        ///
        /// \param level the level at which the option resides
        /// \param name the name of the option
        /// \param val the value of the option
        /// \param len the size of the buffer pointed to by val.
        /// \return returns `true` if the operation completed successfully. On error, `false` is returned. In the case
        /// of an error, `errno` is set.
        /// \note this function is a wrapper around setsockopt(). See setsockopt(2) for additional details.
        bool setOptionRaw(int level, int name, const void *val, socklen_t len) const {
            return setsockopt(fd_, level, name, val, len) == 0;
        }

        /// Gets the raw option on the socket
        ///
        /// \param level the level at which the option resides
        /// \param name the name of the option
        /// \param val the value of the option
        /// \param len the size of the buffer pointed to by val.
        /// \return returns `true` if the operation completed successfully. On error, `false` is returned. In the case
        /// of an error, `errno` is set.
        /// \note this function is a wrapper around getsockopt(). See getsockopt(2) for additional details.
        bool getOptionRaw(int level, int name, void *val, socklen_t *len) const {
            return getsockopt(fd_, level, name, val, len) == 0;
        }

        /// Sets the tcp no delay flag
        ///
        /// \param flag `true` to set `TCP_NODELAY`. `false`
        /// \return returns `true` if the operation completed successfully. On error, `false` is returned. In the case
        /// of an error, `errno` is set.
        [[nodiscard]] bool setNoDelay(bool flag) const {
            int value = flag ? 1 : 0;
            return setOptionRaw(IPPROTO_TCP, TCP_NODELAY, &value, sizeof(int));
        }

        /// Gets the tcp no delay flag
        ///
        /// \return returns `true` if `TCP_NODELAY` is set, `false` if `TCP_NODELAY` is not set.
        [[nodiscard]] bool getNoDelay() const {
            int value(0);
            socklen_t len = sizeof(int);
            getsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &value, &len);
            return value != 0;
        }

        /// Sets the blocking flag
        ///
        /// \param flag `true` to set the socket to blocking mode, `false` to set the socket to non-blocking model.
        /// \return returns `true` if the operation completed successfully. On error, `false` is returned. In the case
        /// of an error, `errno` is set.
        [[nodiscard]] bool setBlocking(bool flag) {
            int opts = fcntl(fd_, F_GETFL, 0);
            if (opts == -1)
                return false;
            // clear non block bit
            int flags = opts & ~O_NONBLOCK;
            if (!flag)
                flags |= O_NONBLOCK;
            blocking_ = flag;
            return fcntl(fd_, F_SETFL, flags) == 0;
        }

        /// Gets the blocking flag
        ///
        /// \return returns `true` if the socket is blocking, `false` if the socket is non-blocking.
        [[nodiscard]] bool getBlocking() const {
            return blocking_;
        }

    private:
        int fd_;
        bool blocking_;
    };

}

#endif //NET_SOCKET_HPP
