#include <net/socket.hpp>
#include "gtest/gtest.h"

namespace {

    TEST(StreamSocketTests, Init) {

        // socket with default constructor
        {
            net::StreamSocket sock;
            EXPECT_NE(-1, sock.fd());
            EXPECT_TRUE(sock.getBlocking());
        }

        // non-blocking socket
        {
            net::StreamSocket sock(false);
            EXPECT_NE(-1, sock.fd());
            EXPECT_FALSE(sock.getBlocking());
        }

    }

    TEST(StreamSocketTests, SetGetOptions) {

        net::StreamSocket sock;

        // toggle tcp no delay
        EXPECT_TRUE(sock.setNoDelay(true));
        EXPECT_TRUE(sock.getNoDelay());
        EXPECT_TRUE(sock.setNoDelay(false));
        EXPECT_FALSE(sock.getNoDelay());

        // toggle blocking
        EXPECT_TRUE(sock.setBlocking(true));
        EXPECT_TRUE(sock.getBlocking());
        EXPECT_TRUE(sock.setBlocking(false));
        EXPECT_FALSE(sock.getBlocking());

    }
}
