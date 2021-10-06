#include <net/tcp_client.hpp>
#include "gtest/gtest.h"

namespace {

    TEST(TcpClientTests, Init) {
        auto s = std::make_unique<net::StreamSocket>();
        net::TcpClient tcpClient(std::move(s));
        EXPECT_TRUE(tcpClient.socket());
    }

}


